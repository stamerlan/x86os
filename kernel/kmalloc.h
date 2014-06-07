/*!
 * \file kmalloc.h
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * \todo What about selfdef std types?
 */

#ifndef KMALLOC_H
#define KMALLOC_H

#include <stddef.h>

void *kmalloc(size_t size);

#endif /* KMALLOC_H */

