/* File: minix.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * Minix fs driver
 * NOTE: Minix fs v1
 */

#include <x86os/types.h>
#include <x86os/log.h>
#include <x86os/string.h>
#include <x86os/fs/filesystems.h>
#include <x86os/fs/minix/minix.h>
#include <x86os/block/bio.h>
#include <x86os/mm/mm.h>
#include <x86os/stat.h>
#include <x86os/fs/node.h>

static struct super_block superblock;

/* struct node_operations
 * Files:
 *	int (*open)(struct fs_node *node, int flags, mode_t mode);
 *	ssize_t (*read)(struct fs_node *node, char *buf, size_t count);
 *	ssize_t (*write)(struct fs_node *node, char *buf, size_t count);
 *	int (*close)(struct fs_node *node);

 * Directories:
 *	struct fs_node* (*lookup)(struct fs_node *node, const char *entry);
 *	int (*create)(struct fs_node *node, const char *entry, 
 *		struct fs_node *new_node);
 *	int (*remove)(struct fs_node *node, const char *entry);
 */

struct node_operations ops = {
	.read = read,
};

/*
static void print_inode(dev_t dev, unsigned int i_no)
{
	struct buffer *buf;
	sector_t i_block = i_no / MINIX_INODES_PER_BLOCK;
	struct buffer *b = bread(dev, inode_table + i_block * 2);
	log_printf("sector: %d ", inode_table + i_block * 2);
	if (!b) {
		log_printf("can't read inode\n");
		return;
	}
	struct minix_inode *inode = &((struct minix_inode *)&b->b_data)
		[i_no % (512 / sizeof(struct minix_inode))];
	log_printf("i_size = %d i_mode = 0x%x\n", inode->i_size, inode->i_mode);
	if (S_ISDIR(inode->i_mode)) {
		log_printf("  DIRECTORY (sector: %d):\n", inode->i_zone[0] * 2);
		buf = bread(dev, inode->i_zone[0] * 2);
		if (!buf) {
			log_printf("   failed to read %d zone at " \
				"%d sector\n", 0, inode->i_zone[0] * 2);
		} else {
			size_t i;
			struct minix_dir_entry *dir = 
				(struct minix_dir_entry*)&buf->b_data;
			for (i = 0; i < 512 / 32; 
				i++, dir = (struct minix_dir_entry*)((char*)dir + 32)) {
				log_printf("   %d (0x%x): inode = %d, name = %s\n",
					i, (char*)dir - buf->b_data, dir->inode, dir->name);
			}
			brelease(buf);
		}

	} else if (S_ISREG(inode->i_mode)) {
		log_printf("  FILE (sector: %d):\n", inode->i_zone[0] * 2);
		buf = bread(dev, inode->i_zone[0] * 2);
		if (!buf) {
			log_printf("   failed to read %d zone at " \
				"%d sector\n", 0, inode->i_zone[0] * 2);
		} else {
			log_printf("   %s\n", buf->b_data);
			brelease(buf);
		}

	}

	brelease(b);
}
*/

static struct fs_node *
get_root(dev_t dev)
{
	struct buffer *sb_buf;
	struct minix_super_block *sb;

	// Read sb
	log_printf("minixfs: get_root on %d\n", dev);

	sb_buf = bread(dev, 2);
	if (!sb_buf) {
		log_printf("minixfs: bread failed\n");
		return NULL;
	}

	sb = (struct minix_super_block *)&sb_buf->b_data;
	log_printf("minixfs: super_block:\n"	\
		" s_ninodes:\t\t%d\n"		\
		" s_nzones:\t\t%d\n"		\
		" s_imap_blocks:\t%d\n"		\
		" s_zmap_blocks:\t%d\n"		\
		" s_firstdatazone:\t%d\n"	\
		" s_log_zone_size:\t%d\n"	\
		" s_max_size:\t\t%d\n"		\
		" s_magic:\t\t0x%x (%s)\n"	\
		" s_state:\t\t%d\n"		\
		" s_zones:\t\t%d\n", sb->s_ninodes, sb->s_nzones, 
		sb->s_imap_blocks, sb->s_zmap_blocks, sb->s_firstdatazone, 
		sb->s_log_zone_size, sb->s_max_size, sb->s_magic, 
		sb->s_magic == MINIX_SUPER_MAGIC2 ? "valid" : "invalid" , 
		sb->s_state, sb->s_zones);

	memmove(&superblock.s_sb, sb, sizeof(struct minix_super_block));
	superblock.s_dev = dev;

	// Read imap and zmap
	if (read_imap(&superblock))
		return NULL;
	if (read_zmap(&superblock)) {
		free_imap(&superblock);
		return NULL;
	}
	brelease(sb_buf);

	struct inode *inode = get_inode(&superblock, MINIX_ROOT_INO);
	return mk_fs_node(inode);
}

struct file_system_type minixfs = {
	.name = "minixfs",
	.get_root = &get_root,
};
