/* File: bio.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef BIO_H
#define BIO_H

#include <x86os/types.h>
#include <x86os/block/buf.h>

void binit();
struct buf *bread(dev_t dev, sector_t sector);
void bwrite(struct buf *buf);

#endif /* BIO_H */

