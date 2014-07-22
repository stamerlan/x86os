/* File: superblock.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef SUPERBLOCK_H
#define SUPERBLOCK_H

#include <x86os/types.h>

struct super_block {
	dev_t		s_dev;
	unsigned long	s_ninodes;
	unsigned long	s_nblocks;
	unsigned int	s_block_size;
	unsigned int	s_log_block_size;

	struct inode	*root;
	struct inode	*mount;
};

#endif /* SUPERBLOCK_H */

