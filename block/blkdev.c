/* File: blkdev.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/types.h>
#include <x86os/spinlock.h>
#include <x86os/log.h>
#include <x86os/mm/mm.h>
#include <x86os/block/blkdev.h>
#include <x86os/block/buffer.h>
#include <x86os/list.h>

// TODO: add device name
struct block_device {
	dev_t dev;
	unsigned int blocksz;
	struct block_device_operations *ops;
	struct list_head bdevs;
};

// TODO: add initialization
static struct {
	struct spinlock lock;
	struct list_head head;
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

	spin_lock(&blkdev_table.lock);
	list_add(&p->bdevs, &blkdev_table.head);
	spin_unlock(&blkdev_table.lock);

	log_printf("block: registred blkdev %d. block_device_operations at " \
			"0x%x\n", p->dev, ops);

	return p->dev;
}

void
unregister_blkdev(dev_t dev)
{
	spin_lock(&blkdev_table.lock);
	struct block_device *p;

	list_for_each_entry(p, &blkdev_table.head, bdevs) {
		if (p->dev == dev) {
			list_del(&p->bdevs);

			// TODO: invoke kfree(p)
			goto out;
		}
	}
out:
	spin_unlock(&blkdev_table.lock);

	log_printf("block: unregistred blkdev %d\n");
}

/* If B_VALID isn't set, invoke read function, set B_VALID
 * NOTE: Buffer must be busy
 */
void
do_blkread(struct buffer *b)
{
	if (!(b->b_flags & B_BUSY))
		// PANIC
		log_printf("panic: do_blkread: buf isn't busy\n");
	if (b->b_flags & B_VALID) {
		log_printf("debug: do_blkread: nothing to do\n");
		return;
	}

	struct block_device *p = NULL;
	spin_lock(&blkdev_table.lock);
	list_for_each_entry(p, &blkdev_table.head, bdevs) {
		if (p->dev == b->b_dev)
			break;
	}
	spin_unlock(&blkdev_table.lock);
	if (&p->bdevs == &blkdev_table.head)
		// Device wasn't found
		return;
	// TODO: IMPORTANT!!! do request before device may be unregistred 
	if (!p->ops->read(b))
		b->b_flags |= B_VALID;
}

/* If B_DIRTY is set, invoke write function, set B_VALID and reset B_DIRTY
 * NOTE: Buffer must be busy
 */
void
do_blkwrite(struct buffer *b)
{
	if (!(b->b_flags & B_BUSY))
		// PANIC
		log_printf("panic: do_blkwrite: buf isn't busy\n");
	if (!(b->b_flags & B_DIRTY)) {
		log_printf("debug: do_blkwrite: nothing to do\n");
		return;
	}

	struct block_device *p = NULL;
	spin_lock(&blkdev_table.lock);
	list_for_each_entry(p, &blkdev_table.head, bdevs) {
		if (p->dev == b->b_dev)
			break;
	}
	spin_unlock(&blkdev_table.lock);
	if (&p->bdevs == &blkdev_table.head)
		// Device wasn't found
		return;
	// TODO: IMPORTANT!!! do request before device unregistred 
	if (!p->ops->write(b)) {
		b->b_flags &= ~B_DIRTY;
		b->b_flags |= B_VALID;
	}
}

