/* File: log.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * TODO: use bochs 0xE9 port for output
 * TODO: use ring buffer for messages
 * TODO: remove any console output
 * TODO: looks like smth with console scrolling
 */

#define LOG_WIDTH	80
#define LOG_HEIGHT	25

#include <stdarg.h>
#include "types.h"
#include "log.h"
#include "asm.h"

static unsigned char xpos = 0;
static unsigned char ypos = 0;
// TODO: use define instead
static char *video = (char *)0xb8000;

/* Reads cursor pos from bios mem
 * NOTE: Should be called before any log operations
 */
void log_init()
{
	xpos = *((unsigned char *) 0x450);
	ypos = *((unsigned char *) 0x451);
}

// Fills all video memory with '\0'
void log_clear()
{
	unsigned int i;
	for (i = 0; i < LOG_WIDTH * LOG_HEIGHT * 2; i++)
		video[i * 2] = '\0';
}

// Setups xpos, ypos, scrolles screen if necessary, sets cursor pos
static void log_newline()
{
	xpos = 0;

	if (ypos == LOG_HEIGHT) {
		// Scrolling
		unsigned int i;
		for (i = 0; i < LOG_WIDTH * (LOG_HEIGHT - 1) * 2; i++)
			video[i] = video[i + LOG_WIDTH * 2];
		// Clear last line
		for (i = LOG_WIDTH * (LOG_HEIGHT - 1) * 2; 
			i < LOG_WIDTH * LOG_HEIGHT * 2; i++)
			video[i] = '\0';
	} else {
		ypos++;
	}
}

// Moves cursor to positon (xpos, ypos)
static void log_move_cursor()
{
	uint16_t pos = ypos * LOG_WIDTH + xpos;
	outb(0x3d4, 14);
	outb(0x3d5, pos >> 8);
	outb(0x3d4, 15);
	outb(0x3d5, pos);
}

// Copy char to video ram, text attribute = 7, moves cursor
void log_putc(char c)
{
	switch (c) {
	case '\n':
		log_newline();
		break;
	case '\t':
		xpos += 8 - (xpos % 8);

		if (xpos >= LOG_WIDTH)
			log_newline();
		break;
	default:
		video[(ypos * LOG_WIDTH + xpos) * 2] = c;
		video[(ypos * LOG_WIDTH + xpos) * 2 + 1] =0x07;

		xpos++;
		if (xpos == LOG_WIDTH)
			log_newline();
		break;
	}
	log_move_cursor();
}

// Print string to log
void log_puts(char *s)
{
	char *p = s;
	while (*p != '\0') {
		log_putc(*p);
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
					log_putc('-');
			}

			log_puts(s);
		} else {
			log_putc(c);
		}
	}
	va_end(argp);
}

