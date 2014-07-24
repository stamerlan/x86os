/* File: vsprintf.c
 * Author: Aleksei Karpenko <nwcdragon.h@gmail.com>
 */

#include <stdarg.h>
#include <string.h>

#define PLUS  1
#define LEFT  2
#define ZERO  4
#define SMALL 8
#define SIGN  16
#define SPACE 32

int vsprintf(char *buf, const char *fmt, va_list args)
{
	char* element;
	int flags;
	for( element = buf; *fmt; fmt++ )
	{
		if ( *fmt != "%" )
		{
			*element++ = *fmt;
			continue;
		}
		
		flags = 0;
		rep:
			++fmt;
			switch (*fmt){
				case '+': flags |= PLUS; goto rep;
				case '-': flags |= LEFT; goto rep;
				case '0': flags |= ZERO; goto rep;
			}
		// size?
		switch ( *fmt ){
			case 'x':
		// add all specifiers
		}

	}
	
	
}

static char* num(char* str, int num, int base, int type)	// parameter (int size) if need size 
{
	char c, sign, tmp[16];
	const char *digits="0123456789ABCDEF";
	int i;
	
	c = (type&ZEROPAD) ? '0' : ' ' ;
	if (type&SMALL) digits="0123456789abcdef";
	if (base<2 || base>16)
		return 0;
	if (type&SIGN && num<0) {
		sign='-';
		num = -num;
	} else
		sign=(type&PLUS) ? '+' : ((type&SPACE) ? ' ' : 0);
	//the other situation
}
