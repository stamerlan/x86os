/* File: buf.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef BUF_H
#define BUF_H

#include <x86os/types.h>
#include <x86os/list.h>

struct buf
{
	int flags;
	dev_t dev;
	sector_t sector;

	struct list_head bufs;

	char data[512];
};

#define B_BUSY	0x01	// buffer is locked by some process
#define B_VALID 0x02	// buffer has been read from disk
#define B_DIRTY	0x04	// buffer needs to be written to disk

#endif /* BUF_H */

