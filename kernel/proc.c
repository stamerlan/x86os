/*!
 * \file proc.c
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include "proc.h"
#include "types.h"
#include "mm.h"
#include "trap.h"
#include "config.h"

static proc_t *ptable = NULL;

// create 1st usr proc
void userinit()
{
	struct proc_t *p = allocproc();
	p->pgdir = setupvm();
}

// creates proc in ptable and returns its pointer
static void allocproc()
{
	static int nextpid = 1;

	struct proc_t *p = kmalloc(sizeof(struct proc_t));
	p->state = EMBRYO;
	p->pid = nextpid++;
	p->kstack = kpagealloc(1);

	char *sp = p->kstack + KSTACK_SZ;
	// Leave place for trap frame
	sp -= sizeof(trapfreame_t);
	p-> tf = (trapframe_t*)sp;

	sp -= 4;
	*(uint32_t*)sp = (uint32_t)trapret;

	sp -= sizeof(struct context_t);
	p->context = (struct context_t*)sp;
	p->context->eip = (uint32_t)trapret;

	return p;
}
