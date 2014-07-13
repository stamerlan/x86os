/* File: blk-dev.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/types.h>
#include <x86os/spinlock.h>
#include <x86os/log.h>
#include <x86os/mm/mm.h>
#include <x86os/block/blk-dev.h>
#include <x86os/block/buf.h>

// TODO: add device name
struct block_device {
	dev_t dev;
	struct block_device_operations *ops;
	struct block_device *next;
};

// TODO: add initialization
static struct {
	struct spinlock lock;
	struct block_device *head;
} blkdev_table;

/* Returns device number
 * TODO: Handle dev_t overflow
 */
static dev_t
alloc_devnr()
{
	static dev_t nr = 1;

	return nr++;
}

/* Registers block device
 * TODO: Multiple devices registration at once
 */
dev_t
register_blkdev(struct block_device_operations * ops)
{
	struct block_device *p = kmalloc(sizeof (struct block_device));
	if (!p)
		return 0;
	p->dev = alloc_devnr();
	if (!p->dev) {
		// TODO: invoke kfree(p)
		return 0;
	}
	p->ops = ops;

	acquire(&blkdev_table.lock);
	p->next = blkdev_table.head;
	blkdev_table.head = p->next;
	release(&blkdev_table.lock);

	return p->dev;
}

void
unregister_blkdev(dev_t dev)
{
	acquire(&blkdev_table.lock);
	struct block_device *p;
	struct block_device *prev = NULL;
	for (p = blkdev_table.head; p != NULL; prev = p, p = p->next) {
		if (p->dev == dev) {
			prev->next = p->next;

			// TODO: invoke kfree(p)
			return;
		}
	}
}

/* If B_VALID isn't set, invoke read function, set B_VALID
 * NOTE: Buffer must be busy
 */
void
do_blkread(struct buf *b)
{
	if (!(b->flags & B_BUSY))
		// PANIC
		log_printf("panic: do_blkread: buf isn't busy\n");
	if (b->flags & B_VALID) {
		log_printf("debug: do_blkread: nothing to do\n");
		return;
	}

	struct block_device *p = NULL;
	acquire(&blkdev_table.lock);
	for (p = blkdev_table.head; p != NULL; p = p->next) {
		if (p->dev == b->dev)
			break;
	}
	release(&blkdev_table.lock);
	if (!p)
		// Device wasn't found
		return;
	// TODO: IMPORTANT!!! do request before device unregistred 
	if (!p->ops->read(b))
		b->flags |= B_VALID;
}

/* If B_DIRTY is set, invoke write function, set B_VALID and reset B_DIRTY
 * NOTE: Buffer must be busy
 */
void
do_blkwrite(struct buf *b)
{
	if (!(b->flags & B_BUSY))
		// PANIC
		log_printf("panic: do_blkwrite: buf isn't busy\n");
	if (!(b->flags & B_DIRTY)) {
		log_printf("debug: do_blkwrite: nothing to do\n");
		return;
	}

	struct block_device *p = NULL;
	acquire(&blkdev_table.lock);
	for (p = blkdev_table.head; p != NULL; p = p->next) {
		if (p->dev == b->dev)
			break;
	}
	release(&blkdev_table.lock);
	if (!p)
		// Device wasn't found
		return;
	// TODO: IMPORTANT!!! do request before device unregistred 
	if (!p->ops->write(b)) {
		b->flags &= ~B_DIRTY;
		b->flags |= B_VALID;
	}
}
