/* File: kernel.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include "log.h"
#include "mm.h"
#include "i8259.h" 
#include "trap.h"
#include "asm.h"
#include "proc.h"
#include "x86.h"

void kmain(long magic, void *mbi) __attribute__((noreturn));
void kmain(long magic, void *mbi)
{
	// TODO: move after mm_init
	log_init();
	mm_init();

	log_printf("x86os starting:\n\n");

	pic_init();
	idt_init();

	userinit();
	pushcli();
	pic_enable(IRQ_TIMER);
	sched();

	for(;;);
}

