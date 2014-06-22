/* File: sched.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/asm.h>
#include <x86os/x86.h>
#include <x86os/proc.h>
#include <x86os/mm/mm.h>

// Kernel context
static struct context *kcontext;

// Implemented in swtch.S
void swtch(struct context **old, struct context *new);

extern struct proc *proc_list;
static struct proc *current;

void yield()
{
	swtch(&current->context, kcontext);
}

// NOTE: Only kernel context
void sched()
{
	for(;;)
	{
		struct proc *p;
		for (p = proc_list; p != NULL; p = p->next)
		{
			if (p->state != RUNNABLE)
				continue;

			current = p;
			switchvm(p);
			p->state = RUNNING;
			swtch(&kcontext, p->context);
			p->state = RUNNABLE;
		}
	}
}

