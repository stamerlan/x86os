/* File: bio.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef BIO_H
#define BIO_H

#include <x86os/types.h>
#include <x86os/block/buffer.h>

#define SECTOR_SIZE	512

void binit();
struct buffer *bread(dev_t dev, sector_t blocknr);
void bwrite(struct buffer *buf);
void brelease(struct buffer *buf);

#endif /* BIO_H */

