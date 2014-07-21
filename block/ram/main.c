/* File: main.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * RAM disk driver
 */

#include <x86os/log.h>
#include <x86os/types.h>
#include <x86os/string.h>
#include <x86os/errno.h>
#include <x86os/mm/mm.h>
#include <x86os/block/blkdev.h>

static dev_t disk_dev;
static char *disk_data;
static sector_t disk_sectors;

static int
read(struct buffer *buf)
{
	if (buf->b_blocknr > disk_sectors)
		return -EFAULT;

	memmove(buf->b_data, disk_data + buf->b_blocknr * 512, 512);

	return 0;
}

static int
write(struct buffer *buf)
{
	return -EFAULT;
}

static struct block_device_operations ops = {
	.read = read,
	.write = write,
};

/* Initialize RAM disk driver
 * @start - start of ramdisk region
 * @end - end of ramdisk region (points to next byte after disk end)
 */
void
init_ramdrv(char *start, char *end)
{
	disk_data = start;
	disk_sectors = (end - start) / 512;

	log_printf("ramdrv: init: start = 0x%x, end = 0x%x\n",
		       start, end);

	disk_dev = register_blkdev(&ops);
	if (!disk_dev)
		// kfree(disk_data)
		return;
	log_printf("ramdrv: init: disk_dev is %d\n", disk_dev);
}
