/* File: bio.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * Buffer cache
 *
 * The buffer cache is a linked list of buffer structures holding cached copies
 * of disk block contents. Caching disk blocks in memory reduces the number of
 * disk reads.
 *
 * Interface:
 *  - To get a buffer for a particaular filesystem block call bread
 *  - After changing buffer data, call bwrite to write it to disk
 *  - Don't use the buffer after calling brelease
 *  - Only one process at a time can use a buffer, so don't keep them longer
 *    than necessary
 *
 * The implementation uses three state flags internally:
 *  - B_BUSY:  the block has been returned from bread and has not been passed
 *             back to brelease
 *  - B_VALID: the buffer data has been read from the disk
 *  - B_DIRTY: the buffer data has been modified and needs to be written to disk
 */

#include <x86os/config.h>
#include <x86os/spinlock.h>
#include <x86os/types.h>
#include <x86os/log.h>
#include <x86os/proc.h>
#include <x86os/block/buffer.h>
#include <x86os/block/blkdev.h>
#include <x86os/mm/mm.h>
#include <x86os/list.h>
#include <x86os/wait_queue.h>

// TODO: Add initialization
static struct {
	struct spinlock lock;
	struct list_head head;
} bcache;

static DECLARE_WAIT_QUEUE_HEAD(bwait);

void
binit()
{
	size_t i;
	struct buffer *p;

	INIT_LIST_HEAD(&bcache.head);

	// TODO: Flag MEM_FS
	p = kmalloc(sizeof(struct buffer));
	if (!p) {
		// PANIC
		log_printf("panic: binit\n");
		return;
	}

	for (i = 0; i < NR_BUF; i++) {
		// TODO: Flag MEM_FS
		p = kmalloc(sizeof(struct buffer));
		if (!p) {
			// PANIC
			log_printf("panic: binit\n");
			return;
		}
		p->b_dev = 0;
		p->b_flags = 0;
		list_add(&p->b_bufs, &bcache.head);
	}
}

/* Look throught bcache for sector on dev. If not found, allocate new block. 
 * In any case, return B_BUSY buffer
 */
static struct buffer *
bget(dev_t dev, sector_t blocknr)
{
	struct buffer *p;

	spin_lock(&bcache.lock);

	for(;;) {
		// Is the buffer already cached?
		list_for_each_entry(p, &bcache.head, b_bufs) {
			if (p->b_dev == dev && p->b_blocknr == blocknr) {
				if (!(p->b_flags & B_BUSY)) {
					p->b_flags |= B_BUSY;
					spin_unlock(&bcache.lock);
					return p;
				}
				// Buf is busy, wait for brelease
				// TODO: wait only for necessary block
				wait(&bwait, &bcache.lock);
				continue;	// search for buffer again
			}
		}

		break;
	}

	// Noncached. Find non-busy and non-dirty block
	list_for_each_entry(p, &bcache.head, b_bufs) {
		if (!(p->b_flags & B_BUSY) && !(p->b_flags & B_DIRTY)) {
			p->b_dev = dev;
			p->b_blocknr = blocknr;
			p->b_flags = B_BUSY;
			spin_unlock(&bcache.lock);
			return p;
		}
	}

	// PANIC
	log_printf("panic: no buffers\n");
	return NULL;
}

// Return a B_BUSY bufffer with the contents of the indicated disk block
struct buffer *
bread(dev_t dev, sector_t blocknr)
{
	struct buffer *p = bget(dev, blocknr);
	if (!(p->b_flags & B_VALID))
		// TODO: ioscheduler?
		do_blkread(p);

	return p;
}

// Writes buf contents to disk. Must be B_BUSY
void
bwrite(struct buffer *buf)
{
	if (!(buf->b_flags & B_BUSY))
		// PANIC
		log_printf("panic: bwrite non-busy buf\n");

	buf->b_flags |= B_DIRTY;
	// TODO: ioscheduler
	do_blkwrite(buf);
}

// Release a B_BUSY buffer
void
brelease(struct buffer *buf)
{
	if (!(buf->b_flags & B_BUSY))
		// PANIC
		log_printf("panic: brelease\n");

	spin_lock(&bcache.lock);
	buf->b_flags &= ~B_BUSY;
	wakeup(&bwait);
	spin_unlock(&bcache.lock);
}
