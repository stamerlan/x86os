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
	struct pde_t* pgdir;		// page table
	char *kstack;			// bottom of kstack for this proc
	enum procstate_t state;		// process state
	int pid;			// process ID
	struct proc_t *parent;		// parent process
	struct proc_t *next;		// next proc in ptable
	struct trapframe_t *tf;		// trap frame for current syscall
	struct context_t *context;	// swtch() here to run process
};

/// TSS format
struct taskstate_t
{
	uint32_t link;		// old ts selector
	uint32_t esp0;		// kstack ptr and selector
	uint16_t ss0;
	uint16_t pad1;
	uint32_t esp1;
	uint16_t ss1;
	uint16_t pad2;
	uint32_t esp2;
	uint16_t ss2;
	uint16_t pad3;

	uint32_t *cr3;		// page directory base
	uint32_t eip;
	uint32_t eflags;
	uint32_t eax;
	uint32_t ecx;
	uint32_t edx;
	uint32_t ebx;
	uint32_t esp;
	uint32_t ebp;
	uint32_t esi;
	uint32_t edi;

	uint16_t es;
	uint16_t pad4;
	uint16_t cs;
	uint16_t pad5;
	uint16_t ss;
	uint16_t pad6;
	uint16_t ds;
	uint16_t pad7;
	uint16_t fs;
	uint16_t pad8;
	uint16_t gs;
	uint16_t pad9;
	uint16_t ldt;
	uint16_t pad10;
	uint16_t t;		// trap on task switch
	uint16_t iomb;		// i/o map base addr
};

void userinit();

#endif /* PROC_H */

