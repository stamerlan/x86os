/* File: bio.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * Buffer cache
 *
 * The buffer cache is a linked list of buf structures holding cached copies
 * of disk block contents. Caching disk blocks in memory reduces the number of
 * disk reads.
 *
 * Interface:
 *  - To get a buffer for a particaular disk block, call bread
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
#include <x86os/block/buf.h>
#include <x86os/block/blk-dev.h>
#include <x86os/mm/mm.h>
#include <x86os/list.h>

// TODO: Add initialization
static struct {
	struct spinlock lock;
	struct list_head head;
} bcache;

void
binit()
{
	size_t i;
	struct buf *p;

	INIT_LIST_HEAD(&bcache.head);

	// TODO: Flag MEM_FS
	p = kmalloc(sizeof (struct buf));
	if (!p) {
		// PANIC
		log_printf("panic: binit\n");
		return;
	}
	p->dev = 0;
	p->flags = 0;
	list_add(&p->bufs, &bcache.head);

	for (i = 0; i < NR_BUF; i++) {
		// TODO: Flag MEM_FS
		p = kmalloc(sizeof (struct buf));
		if (!p) {
			// PANIC
			log_printf("panic: binit\n");
			return;
		}
		p->dev = 0;
		p->flags = 0;
		list_add(&p->bufs, &bcache.head);
	}
}

/* Look throught bcache for sector on dev. If not found, allocate new block. 
 * In any case, return B_BUSY buffer
 */
static struct buf *
bget(dev_t dev, sector_t sector)
{
	struct buf *p;

	spin_lock(&bcache.lock);

	for(;;) {
		// Is the sector already cached?
		list_for_each_entry(p, &bcache.head, bufs) {
			if (p->dev == dev && p->sector == sector) {
				if (!(p->flags & B_BUSY)) {
					p->flags |= B_BUSY;
					spin_unlock(&bcache.lock);
					return p;
				}
				// Buf is busy, wait for brelease
				sleep(p, &bcache.lock);
				continue;	// search for buffer again
			}
		}

		break;
	}

	// Noncached
	list_for_each_entry(p, &bcache.head, bufs) {
		if (!(p->flags & B_BUSY) && !(p->flags & B_DIRTY)) {
			p->dev = dev;
			p->sector = sector;
			p->flags = B_BUSY;
			spin_unlock(&bcache.lock);
			return p;
		}
	}

	// PANIC
	log_printf("panic: no buffers\n");
	return NULL;
}

// Return a B_BUSY buf with the contents of the indicated disk sector
struct buf *
bread(dev_t dev, sector_t sector)
{
	struct buf *p;

	p = bget(dev, sector);
	if (!(p->flags & B_VALID))
		// TODO: ioscheduler
		do_blkread(p);

	return p;
}

// Writes buf contents to disk. Must be B_BUSY
void
bwrite(struct buf *buf)
{
	if (!(buf->flags & B_BUSY))
		// PANIC
		log_printf("panic: bwrite non-busy buf\n");

	buf->flags |= B_DIRTY;
	// TODO: ioscheduler
	do_blkwrite(buf);
}

// Release a B_BUSY buffer
void
brelease(struct buf *buf)
{
	if (!(buf->flags & B_BUSY))
		// PANIC
		log_printf("panic: brelease\n");

	spin_lock(&bcache.lock);
	buf->flags &= ~B_BUSY;
	wakeup(buf);
	spin_unlock(&bcache.lock);
}
