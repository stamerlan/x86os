/* File: log.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#define LOG_WIDTH	80
#define LOG_HEIGHT	25

#include <stdarg.h>
#include <x86os/types.h>
#include <x86os/log.h>
#include <x86os/asm.h>
#include <x86os/vsprintf.h>

static char buf[1024];

// Char to bochs debugger
static void
kputc(char c)
{
	outb(0xe9, c);
}

// Print string to log
static void
kputs(char *s , size_t len)
{
	outsb(0xe9, s, len);
}

/* Format and print string to log
 * NOTE: supported formats: %d, %u, %x, %s, %c
 */
void
log_printf(char *fmt, ...)
{
	va_list args;
	int len;

	va_start(args, fmt);
	len = vsprintf(buf, fmt, args);
	va_end(args);
	kputs(buf, len);

}
