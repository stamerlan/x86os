/*!
 * \file kernel.c
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * \todo add doxygen
 */

#include "log.h"
#include "mm.h"

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

