/* File: inode.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef INODE_H
#define INODE_H

#include <x86os/fs/fs.h>

void iput(struct inode * inode);
struct inode * iget(struct inode * inode);

#endif /* INODE_H */

