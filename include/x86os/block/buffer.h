/* File: buffer.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * Buffers used to represent filesystem buffer in memory.
 */

#ifndef BUFFER_H
#define BUFFER_H

#include <x86os/types.h>
#include <x86os/list.h>

struct buffer
{
	dev_t		b_dev;
	sector_t	b_blocknr;
	char		b_data[512];

	int			b_flags;
	struct list_head	b_bufs;
};

#define B_BUSY	0x01	// buffer is locked by some process
#define B_VALID 0x02	// buffer has been read from disk
#define B_DIRTY	0x04	// buffer needs to be written to disk

#endif /* BUFFER_H */

