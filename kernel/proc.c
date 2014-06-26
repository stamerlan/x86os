/* File: proc.c
 * Author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/proc.h>
#include <x86os/types.h>
#include <x86os/mm/mm.h>
#include <x86os/mm/seg.h>
#include <x86os/trap.h>
#include <x86os/config.h>
#include <x86os/string.h>
#include <x86os/x86.h>
#include <x86os/log.h>
#include <x86os/spinlock.h>

// Process list
struct 
{
	struct spinlock lock;
	struct proc *proc;
} ptable;
static struct proc *current;
static struct context *kcontext;

extern void trapret();
extern void swtch(struct context **old, struct context *new);

/* simple program to run in user mode:
 *.loop:	int	64	; prog2: int 65
 *		jmp	.loop
 */
static char prog[] = {0xcd, 0x40, 0xeb, 0xfc};
static char prog2[] = {0xcd, 0x41, 0xeb, 0xfc};

// On first scheduling new process should release ptable.lock
static void forkret()
{
	release(&ptable.lock);

	// Return to trapret
}

/* Creates new process in ptable, returns its pointer
 * setups kstack, pid, context
 */
static void *allocproc()
{
	static int nextpid = 1;

	struct proc *p = kmalloc(sizeof(struct proc));
	p->state = EMBRYO;
	p->pid = nextpid++;
	p->kstack = kpagealloc(KSTACK_SZ / PAGE_SZ);
	log_printf("debug: allocproc(): kstack = 0x%x\n", p->kstack);

	char *sp = p->kstack + KSTACK_SZ;
	// Leave place for trap frame
	sp -= sizeof(struct trapframe);
	p->tf = (struct trapframe*)sp;
	log_printf("debug: allocproc(): tf = 0x%x\n", (uint32_t)p->tf);

	// Return from forkret to trapret
	sp -= 4;
	*(uint32_t*)sp = (uint32_t)trapret;
	log_printf("debug: allocproc(): ret addr ptr = 0x%x, ret add = %x\n",
			sp, *(uint32_t*)sp);

	sp -= sizeof(struct context);
	p->context = (struct context*)sp;
	log_printf("debug: allocproc(): context = 0x%x\n", p->context);
	p->context->eip = (uint32_t)forkret;

	acquire(&ptable.lock);
	p->next = ptable.proc;
	ptable.proc = p;
	release(&ptable.lock);

	return p;
}

/* Enter scheduler.
 * NOTE: ptable.lock must be held and current->state already changed.
 */
static void sched()
{
	// TODO: add check is ptable.lock is holding.
	// TODO: add check is current->state changed (!= RUNNING).
	swtch(&current->context, kcontext);
}

/* It loops:
 *  - choose process to run
 *  - swtch to start running choosen process
 *  - eventually that process transfers control via swtch back to scheduler
 * NOTE: Only kernel context.
 * NOTE: Never returns.
 * TODO: Add compiler "never return".
 */
void scheduler()
{
	for(;;)
	{
		struct proc *p;

		acquire(&ptable.lock);
		for (p = ptable.proc; p != NULL; p = p->next)
		{
			if (p->state != RUNNABLE)
				continue;

			current = p;
			switchvm(p);
			p->state = RUNNING;
			swtch(&kcontext, p->context);
			switchkvm();
			p->state = RUNNABLE;
			current = NULL;
		}
		release(&ptable.lock);
	}
}

// Creates 2 user proces
void userinit()
{
	ptable.proc = NULL;

	struct proc *p = allocproc();
	p->pgdir = setupvm();
	log_printf("debug: userinit(): process pde = 0x%x\n", p->pgdir);
	char *mem = kpagealloc(1);
	kmap(p->pgdir, mem, (void*)0x0);
	p->sz = PAGE_SZ;
	memset(p->tf, 0, sizeof(struct trapframe));
	p->tf->cs = (SEG_UCODE << 3) | DPL_USR;
	p->tf->ds = (SEG_UDATA << 3) | DPL_USR;
	p->tf->es = p->tf->ds;
	p->tf->ss = p->tf->es;
	p->tf->eflags = FL_IF;
	p->tf->esp = PAGE_SZ;
	p->tf->eip = 0;
	// TODO: Remove this part
	p->tf->eax = 0xAAAAAAAA;
	p->tf->ebx = 0xBBBBBBBB;
	p->tf->ecx = 0xCCCCCCCC;
	p->tf->edx = 0xDDDDDDDD;
	p->tf->edi = 0xD1D1D1D1;
	p->tf->esi = 0xC1C1C1C1;
	p->tf->ebp = 0xB0B0B0B0;
	p->tf->gs = p->tf->fs = p->tf->ds;

	p->parent = NULL;

	memmove(mem, prog, sizeof(prog));

	p->state = RUNNABLE;

	// Create 2nd usr proc
	p = allocproc();
	p->pgdir = setupvm();
	log_printf("debug: userinit(): process2 pde = 0x%x\n", p->pgdir);
	mem = kpagealloc(1);
	kmap(p->pgdir, mem, (void*)0x00);
	p->sz = PAGE_SZ;
	memset(p->tf, 0, sizeof(struct trapframe));
	p->tf->cs = (SEG_UCODE << 3) | DPL_USR;
	p->tf->ds = (SEG_UDATA << 3) | DPL_USR;
	p->tf->es = p->tf->ds;
	p->tf->ss = p->tf->es;
	p->tf->eflags = FL_IF;
	p->tf->esp = PAGE_SZ;
	p->tf->eip = 0;
	// TODO: Remove this part
	p->tf->eax = 0xAAAAAAAA;
	p->tf->ebx = 0xBBBBBBBB;
	p->tf->ecx = 0xCCCCCCCC;
	p->tf->edx = 0xDDDDDDDD;
	p->tf->edi = 0xD1D1D1D1;
	p->tf->esi = 0xC1C1C1C1;
	p->tf->ebp = 0xB0B0B0B0;
	p->tf->gs = p->tf->fs = p->tf->ds;

	p->parent = NULL;

	memmove(mem, prog2, sizeof(prog2));

	p->state = RUNNABLE;
}

// Schedule next process.
void yield()
{
	acquire(&ptable.lock);
	current->state = RUNNABLE;
	sched();
	release(&ptable.lock);
}

/* Atomically release lock and sleep on chan.
 * Reacquires lock when awakened.
 */
void sleep(void *chan, struct spinlock *lock)
{
	acquire(&ptable.lock);
	release(lock);

	current->chan = chan;
	current->state = SLEEPING;

	sched();

	// Waking up
	current->chan = NULL;
	acquire(lock);
	release(&ptable.lock);
}

// Wake up all processes sleeping on chan.
void wakeup(void *chan)
{
	acquire(&ptable.lock);

	struct proc *p;
	for (p = ptable.proc; p != NULL; p = p->next)
		if (p->state == SLEEPING && p->chan == chan)
			p->state = RUNNABLE;

	release(&ptable.lock);
}
