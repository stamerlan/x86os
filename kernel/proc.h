/*!
 * \file proc.h
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef PROC_H
#define PROC_H

#include "types.h"
#include "mm.h"
#include "trap.h"

struct context_t
{
	uint32_t edi;
	uint32_t esi;
	uint32_t ebx;
	uint32_t ebp;
	uint32_t eip;
};

enum procstate_t { EMBRYO, RUNNABLE, RUNNING };

// Per-process state
struct proc_t
{
	uint32_t sz;			// size of process memory (bytes)
	pde_t* pgdir;			// page table
	char *kstack;			// bottom of kstack for this proc
	enum procstate_t state;		// process state
	int pid;			// process ID
	struct proc_t *parent;		// parent process
	struct proc_t *next;		// next proc in ptable
	struct trapframe_t *tf;		// trap frame for current syscall
	struct context_t *context;	// swtch() here to run process
};

#endif /* PROC_H */

