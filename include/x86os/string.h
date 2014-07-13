/* File: string.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef STRING_H
#define STRING_H

#include <x86os/types.h>

void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);
size_t strlen(const char *s);
int strcmp(const char *s1, const char *s2);
int strncmp(const char *s1, const char *s2, size_t n);

#endif /* STRING_H */
