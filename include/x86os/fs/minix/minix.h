/* File: minix.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef MINIX_FS_H
#define MINIX_FS_H

#include <x86os/types.h>

#define BLOCK_SIZE	1024

struct minix_super_block {
	uint16_t	s_ninodes;	/* Number of inodes */
	uint16_t	s_nzones;	/* Number of data zones */
	uint16_t	s_imap_blocks;	/* Space used by inode map (blocks) */
	uint16_t	s_zmap_blocks;	/* Space used by zone map (blocks) */
	uint16_t	s_firstdatazone;/* First zpne with "file" data */
	uint16_t	s_log_zone_size;/* Size of data zone =
					    (1024 << s_log_zone_size) */
	uint32_t	s_max_size;	/* Maximum file size (bytes) */
	uint16_t	s_magic;	/* Minix 14/30 ID number */
	uint16_t	s_state;	/* Mount state, was it 
					    cleanly unmounted */
};

#endif /* MINIX_FS_H */

