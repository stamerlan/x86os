/* File: minix.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * Minix fs driver
 */

#include <x86os/types.h>
#include <x86os/log.h>
#include <x86os/fs/filesystems.h>
#include <x86os/fs/minix/minix.h>
#include <x86os/block/bio.h>

static struct fs_node *
get_root(dev_t dev)
{
	log_printf("minixfs: get_root on %d\n", dev);

	struct buf *b = bread(dev, 3);
	if (!b) {
		log_printf("minixfs: bread failed\n");
		return NULL;
	}

	struct minix_super_block *sb = (struct minix_super_block *)&b->data;
	log_printf("minixfs: super block:\n"	\
		" s_ninodes:\t%d\n"		\
		" s_nzones:\t%d\n"		\
		" s_imap_blocks:\t%d\n"		\
		" s_zmap_blocks:\t%d\n"		\
		" s_firstdatazone:\t%d\n"	\
		" s_log_zone_size:\t%d\n"	\
		" s_max_size:\t%d\n"		\
		" s_magic:\t0x%x\n"		\
		" s_state:\t%d\n", sb->s_ninodes, sb->s_nzones,
		sb->s_imap_blocks, sb->s_zmap_blocks, sb->s_firstdatazone,
		sb->s_log_zone_size, sb->s_max_size, sb->s_magic, sb->s_state);

	return NULL;
}


struct file_system_type minixfs = {
	.name = "minixfs",
	.get_root = &get_root,
};
