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

	lidt(idt, sizeof(idt));
}

void trap(struct trapframe_t* tf)
{
	if (tf->trapno == T_IRQ0 + IRQ_TIMER)
		log_printf("irq0: timer int\n");
}

