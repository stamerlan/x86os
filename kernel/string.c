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

/* The strlen() function calculates the length of the string s, excluding the
 * terminating null byte ('\0')
 */
size_t strlen(const char *s)
{
	size_t n;

	for (n = 0; s[n]; n++)
		/* nothing */;

	return n;
}

/* The strcmp() function compares the two strings s1 and s2. It returns an
 * integer less than, equal to, or greater than thero if s1 is found, 
 * respectively, to be less than, to match, or be greater than s2.
 */
int strcmp(const char *s1, const char *s2)
{
	while (*s1 && (*s1 == *s2))
	{
		s1++;
		s2++;
	}

	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

/* The strncmp() function is similar to strcmp, except it compares the only
 * first (at most) n bytes of s1 and s2
 */
int strncmp(const char *s1, const char *s2, size_t n)
{
	while (n > 0 && *s1 && *s1 == *s2)
	{
		n--;
		s1++;
		s2++;
	}
	if (n == 0)
		return 0;
	return *(const unsigned char*)s1 - *(const unsigned char*)s2;
}

