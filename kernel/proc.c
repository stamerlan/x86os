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

// Process list
struct proc *proc_list = NULL;

extern void trapret();

/* simple program to run in user mode:
 *.loop:	int	64	; prog2: int 65
 *		jmp	.loop
 */
static char prog[] = {0xcd, 0x40, 0xeb, 0xfc};
static char prog2[] = {0xcd, 0x41, 0xeb, 0xfc};

/* creates proc in ptable and returns its pointer
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

	/*
	sp -= 4;
	*(uint32_t*)sp = (uint32_t)trapret;
	log_printf("debug: allocproc(): ret addr ptr = 0x%x, ret add = %x\n",
			sp, *(uint32_t*)sp);
	*/

	sp -= sizeof(struct context);
	p->context = (struct context*)sp;
	log_printf("debug: allocproc(): context = 0x%x\n", p->context);
	p->context->eip = (uint32_t)trapret;

	p->next = proc_list;
	proc_list = p;

	return p;
}

// create 1st usr proc
void userinit()
{
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

