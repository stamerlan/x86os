/*!
 * \file kernel.c
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * \details initializes memory manager and logging module
 */

#include "log.h"
#include "mm.h"
#include "i8259.h" 
#include "trap.h"
#include "asm.h"
#include "proc.h"
#include "x86.h"

extern struct pde_t *kpde;
/*!
 * \breif kernel entry point
 * \param magic - magic value
 * \param mbi - multiboot information
 * \sa https://www.gnu.org/software/grub/manual/multiboot/multiboot.html
 * \note arguments get from startup code
 * \note no return
 */
void kmain(long magic, void *mbi) __attribute__((noreturn));
void kmain(long magic, void *mbi)
{
	/// \todo move after mm_init
	log_init();
	mm_init();

	log_printf("x86os starting:\n\n");

	log_printf("debug: kmain(): kernel pde = 0x%x\n", (uint32_t)kpde);

	pic_init();
	idt_init();

	userinit();
	pushcli();
	pic_enable(IRQ_TIMER);
	sched();

	for(;;);
}

