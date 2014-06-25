/* File: block.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef BLOCK_H
#define BLOCK_H

#include <x86os/types.h>

struct block_device_operations
{
	int (*bread)(sector_t sector, size_t sect_nr, char *buf);
	int (*bwrite)(sector_t sector, size_t sect_nr, char *buf);
};

/* Register block device
 * Return: device number or 0 if error occured
 */
dev_t reg_blkdev(char *name, struct block_device_operations *ops);

/* Read data from block device
 * Params:
 *   device - device number
 *   sector - sector to start reading from
 *   sect_nr - sectors count to read
 *   buf - buffer to store read data
 * Returns 0 - error
 *
 * NOTE: sector is 512 bytes long (see sector_t)
 * NOTE: buffer size must be sect_nr * 512 bytes
 * NOTE: buffer must be present in kernel memory
 */
int bread(dev_t device, sector_t sect, size_t sect_nr, char* buf);

/* Write data to block device
 * Params:
 *   device - device number
 *   sector - sector to start writing to
 *   sect_nr - sectors count to write
 *   buf - contains data for writing
 * Returns 0 - error
 *
 * NOTE: sector is 512 bytes long (see sector_t)
 * NOTE: buffer size must be sect_nr * 512 bytes
 * NOTE: buffer must be present in kernel memory
 */
int bwrite(dev_t device, sector_t sect, size_t sect_nr, char* buf);

#endif /* BLOCK_H */

