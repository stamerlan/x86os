/* File: string.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/types.h>

/* Fill memory with a constant byte
 * Detailed: Fills the first n bytes of memory area pointed to by s with
 * the constant byte c
 * Return: A pointer to the memory area s
 * TODO: use asm stosb and stosl
 */
void *memset(void *s, int c, size_t n)
{
	size_t i;
	for (i = 0; i < n; i++)
		*(char*)(s + i) = c;

	return s;
}

/* Copy memory area
 * Detailed: Copies n bytes from memory area src to memory area dest.
 * The memory areas may overlap: copying takes place as though the bytes in
 * src are first copied into a temporary array that does not overlap src
 * or dest, and the bytes are then copied from the temporary array to dest
 * Return: A pointer to dest
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

