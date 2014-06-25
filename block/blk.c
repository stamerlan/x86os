/* File: blk.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/types.h>
#include <x86os/mm/mm.h>
#include <x86os/block/block.h>
#include <x86os/log.h>

struct block_dev
{
	char *name;
	struct block_dev *next;
	dev_t nr;
	struct block_device_operations *ops;
};

// TODO: Use rb-tree instead list
static struct block_dev *block_dev_list = NULL;

/* Allocates next free device number
 * TODO: Device number releasing
 * TODO: Allocate multiple device numbers at once
 */
static dev_t alloc_devnr()
{
	static dev_t nr = 1;

	return nr++;
}

/* Register block device
 * Return: device number or 0 if error occured
 */
dev_t reg_blkdev(char *name, struct block_device_operations *ops)
{
	struct block_dev *p = kmalloc(sizeof(struct block_dev));
	if (!p)
	{
		return 0;
	}

	p->nr = alloc_devnr();
	p->ops = ops;
	p->name = name;
	p->next = block_dev_list;
	block_dev_list = p;

	return p->nr;
}

int bread(dev_t device, sector_t sect, size_t sect_nr, char* buf)
{
	struct block_dev *p;
	log_printf("debug: bread: device = %d sect = %d sect_nr = %d " \
			"buf = 0x%x\n", device, sect, sect_nr, buf);

	for(p = block_dev_list; p != NULL; p = p->next)
	{
		if (p->nr == device)
			return p->ops->bread(sect, sect_nr, buf);
	}

	// Device wasn't found
	return 0;
}

int bwrite(dev_t device, sector_t sect, size_t sect_nr, char* buf)
{
	struct block_dev *p;

	for(p = block_dev_list; p != NULL; p = p->next)
	{
		if (p->nr == device)
			return p->ops->bwrite(sect, sect_nr, buf);
	}

	// Device wasn't found
	return 0;
}

