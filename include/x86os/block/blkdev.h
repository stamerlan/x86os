/* File: blkdev.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef BLKDEV_H
#define BLKDEV_H

#include <x86os/block/buffer.h>

struct block_device_operations
{
	int (*read)(struct buffer*);
	int (*write)(struct buffer*);
};

dev_t register_blkdev(struct block_device_operations *ops);
void unregister_blkdev(dev_t dev);
void do_blkread(struct buffer *b);
void do_blkwrite(struct buffer *b);

#endif /* BLKDEV_H */

