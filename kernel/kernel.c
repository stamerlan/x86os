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
	mm_init();

	log_init();
	log_printf("x86os starting:\n\n");

	pic_init();
	idt_init();
	pic_enable(IRQ_TIMER);

	log_printf("mm: kpagealloc(1): 0x%x\n", kpagealloc(1));
	log_printf("mm: kmalloc(10): 0x%x\n", kmalloc(10));
	log_printf("mm: kmalloc(1): 0x%x\n", kmalloc(1));
	log_printf("mm: kpagealloc(2): 0x%x\n", kpagealloc(2));
	log_printf("mm: kmalloc(1): 0x%x\n", kmalloc(1));

	//sti();

	for(;;);
}

