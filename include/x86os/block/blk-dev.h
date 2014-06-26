/* File: blk-dev.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef BLKDEV_H
#define BLKDEV_H

#include <x86os/block/buf.h>

struct block_device_operations
{
	int (*read)(struct buf*);
	int (*write)(struct buf*);
};

dev_t register_blkdev(struct block_device_operations *ops);
void unregister_blkdev(dev_t dev);
void do_blkread(struct buf *b);
void do_blkwrite(struct buf *b);

#endif /* BLKDEV_H */

