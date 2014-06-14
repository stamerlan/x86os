/*!
 * \file string.c
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include "types.h"
#include "string.h"

/*!
 * \breif fill memory with a constant byte
 * \details fills the first \a n bytes of memory area pointed to by \a s with
 * the constant byte \a c
 * \return returns a pointer to the memory area \a s
 * \todo use asm stosb and stosl
 */
void *memset(void *s, int c, size_t n)
{
	size_t i;
	for (i = 0; i < n; i++)
		*(char*)(s + i) = c;

	return s;
}

/*!
 * \breif copy memory area
 * \details copies \a n bytes from memory area \a src to memory area \a dest.
 * The memory areas may overlap: copying takes place as though the bytes in
 * \a src are first copied into a temporary array that does not overlap \a src
 * or \a dest, and the bytes are then copied from the temporary array to \a dest
 * \return a pointer to \a dest
 */
void *memmove(void *dest, const void *src, size_t n)
{
	const char *s;
	char *d;

	s = src;
	d = dest;
	if(s < d && s + n > d)
	{
		s += n;
		d += n;
		while(n-- > 0)
			*--d = *--s;
	}
       	else
	{
		while(n-- > 0)
			*d++ = *s++;
	}

	return dest;
}

