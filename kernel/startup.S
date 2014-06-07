/*!
 * \file startup.S
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * \breif Contains multiboot header, here bootloader pass controll.
 */

#include "config.h"

/// \todo maybe use multiboot.h?
#define MB_HEADER_MAGIC 0x1BADB002
#define MB_HEADER_FLAGS 0x00000003

.text

.globl _start
_start:
	jmp	mb_entry

	/* Multiboot header */
	.align	4
	.long	MB_HEADER_MAGIC                      /* magic */
	.long	MB_HEADER_FLAGS                      /* flags */
	.long	-(MB_HEADER_MAGIC + MB_HEADER_FLAGS) /* checksum */

mb_entry:
	/* Set up stack */
	leal	(stack + KSTACK_SZ), %esp

	pushl	%ebx
	pushl	%eax
	call	kmain
	/* Halt */
loop:
	hlt
	jmp	loop

/* .bss */
.comm	stack, KSTACK_SZ

