/* File: string.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef STRING_H
#define STRING_H

#include <x86os/types.h>

void *memset(void *s, int c, size_t n);
void *memmove(void *dest, const void *src, size_t n);

#endif /* STRING_H */
