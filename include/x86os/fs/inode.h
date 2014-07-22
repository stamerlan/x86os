/* File: inode.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef INODE_H
#define INODE_H

#include <x86os/types.h>

struct inode {
	unsigned long	i_no;
	mode_t		i_mode;
	uid_t		i_uid;
	gid_t		i_gid;
	size_t		i_size;
	unsigned long	i_time;
	unsigned int	i_nlinks;

	struct super_block	*i_sb;
};

#endif /* INODE_H */

