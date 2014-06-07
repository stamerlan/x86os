/*!
 * \file kmalloc.c
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include "kmalloc.h"

static void *free_ptr = (void*)0x200000;

void* kmalloc(size_t size)
{
	void *ret = free_ptr;
	free_ptr += size;

	return ret;
}

