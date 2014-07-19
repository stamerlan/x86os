/* File: trap.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/types.h>
#include <x86os/trap.h>
#include <x86os/asm.h>
#include <x86os/i8259.h>
#include <x86os/log.h>
#include <x86os/proc.h>

static struct gatedesc idt[256];
extern uint32_t vectors[];	// in vectors.S: array of 256 entry points

void
idt_init()
{
	int i;
	for (i = 0; i < 256; i++)
		SETGATE(idt[i], 0, SEG_KCODE << 3, vectors[i], DPL_SYS);
	SETGATE(idt[T_SYSCALL], 1, SEG_KCODE << 3, vectors[T_SYSCALL], DPL_USR);
	// TODO: remove me
	SETGATE(idt[T_SYSCALL + 1], 1, SEG_KCODE << 3, vectors[T_SYSCALL + 1],
		DPL_USR);

	lidt(idt, sizeof (idt));
}

void
trap(struct trapframe *tf)
{
	switch (tf->trapno) {
	case T_IRQ0 + IRQ_TIMER:
		yield(TASK_RUNNING);
		break;
	case T_SYSCALL:
		outb(0xe9, 'A');
		break;
	case T_SYSCALL + 1:	// TODO: remove me
		outb(0xe9, 'B');
		break;
	default:
		log_printf("unhandled trap 0x%x\n", tf->trapno);
	}
}
