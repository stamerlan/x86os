/* File: log.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#define LOG_WIDTH	80
#define LOG_HEIGHT	25

#include <stdarg.h>
#include <x86os/types.h>
#include <x86os/log.h>
#include <x86os/asm.h>

// Char to bochs debagger
static void kputc(char c)
{
	outb(0xe9, c);
}

// Print string to log
void log_puts(char *s)
{
	char *p = s;
	while (*p != '\0') {
		kputc(*p);
		p++;
	}
}

/* Format and print string to log
 * NOTE: supported formats: %d, %u, %x, %s, %c
 */
void log_printf(char *fmt, ...)
{
	char c;                        // current symbol in fmt
	signed int d;                  // signed number
	unsigned int u;                // unsigned number
	unsigned int base;             // number base
	char negative;                 // is number negative
	static char x2c[] = "0123456789ABCDEF";
	char ascii[11];                // buffer to print number
	char *s;                       // str to print
	va_list argp;                  // arguments

	va_start(argp, fmt);

	while ((c = *fmt++) != '\0') {
		if (c == '%') {
			s = NULL;

			switch (c = *fmt++) {
			case 'd':      // Signed decimal
				d = va_arg(argp, signed int);
				if (d < 0) {
					u = -d;
					negative = 1;
				} else {
					u = d;
					negative = 0;
				}
				base = 10;
				break;
			case 'u':      // Unsigned decimal
				u = va_arg(argp, unsigned int);
				base = 10;
				negative = 0;
				break;
			case 'x':      // Heximal
				u = va_arg(argp, unsigned int);
				base = 16;
				negative = 0;
				break;
			case 's':      // String
				s = va_arg(argp, char*);
				if (s == NULL)
					s = "(null)";
				break;
			case 'c':      // Symbol
				s = "?";
				s[0] = va_arg(argp, int);
			default:       // Unknown
				s = "%?";
				s[1] = c;
			}

			if (s == NULL) {
				// Convert u to string
				s = ascii + sizeof(ascii) - 1;
				*s = 0;
				do {
					*--s = x2c[u % base];
				} while ((u /= base) > 0);

				if (negative)
					kputc('-');
			}

			log_puts(s);
		} else {
			kputc(c);
		}
	}
	va_end(argp);
}

