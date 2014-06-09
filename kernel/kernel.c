/*!
 * \file kernel.c
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * \details initializes memory manager and logging module
 */

#include "log.h"
#include "mm.h"

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

	int a, d;

	asm volatile ("cpuid" : "=a"(a), "=d"(d) : "0"(0x80000001) : "ebx", "ecx");	

	log_printf("cpuid: edx = 0x%x\n", d);

	for(;;);
}

