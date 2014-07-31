/* File: proc.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef PROC_H
#define PROC_H

#include <x86os/types.h>
#include <x86os/mm/page.h>
#include <x86os/trap.h>
#include <x86os/spinlock.h>
#include <x86os/config.h>
#include <x86os/list.h>
#include <x86os/fs/node.h>

#define suser() (current->uid == 0)

struct context {
	uint32_t edi;
	uint32_t esi;
	uint32_t ebx;
	uint32_t ebp;
	uint32_t eip;
};

#define TASK_RUNNABLE	0
#define TASK_RUNNING	1
#define TASK_EMBRYO	2
#define TASK_SLEEPING	3

// Per-process state
struct task_struct {
	size_t sz;		// size of process memory (bytes)
	struct pde *pgdir;	// page table
	char *kstack;		// bottom of kstack for this process
	int state;
	int exit_code, exit_signal;
	int pid;		// process ID
	struct trapframe *tf;	// trap frame for current syscall
	struct context *context;	// swtch() here to run process
	int errno;
	uid_t uid;
	struct task_struct *parent;	// parent process
	struct list_head tasks;
	struct fs_node *pwd;
};

// TSS format
struct tss_struct {
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
void scheduler();
void yield(int task_state);

extern struct task_struct *current;

#endif				/* PROC_H */
