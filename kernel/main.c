/* File: main.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/log.h>
#include <x86os/mm/mm.h>
#include <x86os/i8259.h>
#include <x86os/trap.h>
#include <x86os/x86.h>
#include <x86os/proc.h>
#include <x86os/fs/fs.h>

void init_testfs();

void kmain(long magic, void *mbi) __attribute__((noreturn));
void kmain(long magic, void *mbi)
{
	// TODO: move after mm_init
	mm_init();

	log_printf("x86os starting:\n\n");

	pic_init();
	idt_init();
	userinit();

	pushcli();
	pic_enable(IRQ_TIMER);

	mount_root(1);
	char buf[64];
	sys_read("/test", buf, 64);
	log_printf("debug: read from fs: %s\n", buf);

	scheduler();

	for(;;);
}

