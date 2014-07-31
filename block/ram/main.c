/* File: main.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * RAM disk driver
 */

#include <x86os/types.h>
#include <x86os/string.h>
#include <x86os/errno.h>
#include <x86os/mm/mm.h>
#include <x86os/block/blk-dev.h>

#define DISK_SECT	8
#define DISK_SIZE	DISK_SECT * 512

static dev_t disk_dev;
static char *disk_data;

static int read(struct buf *buf)
{
	if (buf->sector > DISK_SECT)
		return -EFAULT;

	memmove(buf->data, disk_data + buf->sector * 512, 512);

	return 0;
}

static int write(struct buf *buf)
{
	return -EFAULT;
}

static struct block_device_operations ops = {
	.read = read,
	.write = write,
};

void init_ramdrv()
{
	disk_data = kmalloc(DISK_SIZE);
	if (!disk_data)
		return;
	disk_dev = register_blkdev(&ops);
	if (!disk_dev)
		// kfree(disk_data)
		return;
}
