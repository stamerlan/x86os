/* File: main.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <multiboot.h>
#include <x86os/log.h>
#include <x86os/mm/mm.h>
#include <x86os/i8259.h>
#include <x86os/trap.h>
#include <x86os/x86.h>
#include <x86os/proc.h>
#include <x86os/block/bio.h>
#include <x86os/fs/fs.h>

void init_testfs();
void init_ramdrv(char *, char *);

void kmain(long magic, multiboot_info_t *mbi) __attribute__ ((noreturn));
void
kmain(long magic, multiboot_info_t *mbi)
{
	if (mbi->flags & 0x4) {
		mm_init(mbi->mods_count, (multiboot_module_t*) mbi->mods_addr);

		size_t i;
		multiboot_module_t *mod;
		for (i = 0, mod = (multiboot_module_t *)mbi->mods_addr; 
		     i < mbi->mods_count; i++, mod++) {
			log_printf("module: start = 0x%x, end = 0x%x, " \
				"cmdline = %s\n", mod->mod_start, mod->mod_end,
				mod->cmdline);
		}
	}
	else {
		mm_init(0, NULL);
	}

	pic_init();
	idt_init();
	userinit();

	pushcli();
	pic_enable(IRQ_TIMER);
	binit();

	init_ramdrv(NULL, NULL);

	//scheduler();

	for (;;) ;
}
