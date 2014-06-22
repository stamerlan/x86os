/* File: x86.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include "x86.h"
#include "asm.h"
#include "log.h"

static int ncli = 0;

void pushcli()
{
	cli();

	ncli++;
}

void popcli()
{
	if (--ncli < 0)
		log_printf("panic: popcli()\n");
	if (ncli == 0)
		sti();
}

