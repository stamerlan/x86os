/*!
 * \file trap.c
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include "types.h"
#include "trap.h"
#include "asm.h"
#include "i8259.h"
#include "log.h"

static struct gatedesc_t idt[256];
extern uint32_t vectors[];	// in vectors.S: array of 256 entry points

void idt_init()
{
	int i;
	for (i = 0; i < 256; i++)
		SETGATE(idt[i], 0, SEG_KCODE << 3, vectors[i], DPL_SYS);
	SETGATE(idt[T_SYSCALL], 1, SEG_KCODE << 3, vectors[T_SYSCALL], DPL_USR);
	/// \todo remove me
	SETGATE(idt[T_SYSCALL + 1], 1, SEG_KCODE << 3, vectors[T_SYSCALL + 1], DPL_USR);

	lidt(idt, sizeof(idt));
}

void trap(struct trapframe_t* tf)
{
	switch (tf->trapno)
	{
	case T_DIVIDE:
		log_printf("divied error\n");
		break;
	case T_DEBUG:
		log_printf("debug exception\n");
		break;
	case T_NMI:
		log_printf("non-maskable interrupt\n");
		break;
	case T_BRKPT:
		log_printf("breakpoint\n");
		break;
	case T_OFLOW:
		log_printf("overflow\n");
		break;
	case T_BOUND:
		log_printf("bounds check\n");
		break;
	case T_ILLOP:
		log_printf("illegal opcode\n");
		break;
	case T_DEVICE:
		log_printf("device not available\n");
		break;
	case T_DBLFLT:
		log_printf("double fault\n");
		break;
	case T_TSS:
		log_printf("invalid task switch segment\n");
		break;
	case T_SEGNP:
		log_printf("segment not present\n");
		break;
	case T_STACK:
		log_printf("stack exception\n");
		break;
	case T_GPFLT:
		log_printf("general protection fault\n");
		break;
	case T_PGFLT:
		log_printf("page fault\n");
		break;
	case T_FPERR:
		log_printf("floating point error\n");
		break;
	case T_ALIGN:
		log_printf("aligment check\n");
		break;
	case T_MCHK:
		log_printf("machine check\n");
		break;
	case T_SIMDERR:
		log_printf("SIMD floating point\n");
		break;
	case T_IRQ0 + IRQ_TIMER:
		yield();
		break;
	case T_SYSCALL:
		outb(0xe9, 'A');
		break;
	case T_SYSCALL + 1:	/// \todo remove me
		outb(0xe9, 'B');
		break;
	default:
		log_printf("unhandled trap 0x%x\n", tf->trapno);
	}
}

