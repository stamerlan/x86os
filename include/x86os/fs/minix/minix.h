/* File: minix.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * Minix filesystem v1 constants and structures
 */

#ifndef MINIX_FS_H
#define MINIX_FS_H

#include <x86os/types.h>
#include <x86os/fs/node.h>

#define MINIX_ROOT_INO	1

#define BLOCK_SIZE	1024
#define MINIX_INODES_PER_BLOCK ((BLOCK_SIZE)/(sizeof(struct minix_inode)))

#define MINIX_SUPER_MAGIC2	0x138F		/* minix v1 fs, 30 char names */

/*
 * This is the original minix inode layout on disk.
 * Note the 8-bit gid and atime and ctime.
 */
struct minix_inode {
	uint16_t i_mode;
	uint16_t i_uid;
	uint32_t i_size;
	uint32_t i_time;
	uint8_t  i_gid;
	uint8_t  i_nlinks;
	uint16_t i_zone[9];
} __attribute__((packed));

/*
 * minix super-block data on disk
 */
struct minix_super_block {
	uint16_t s_ninodes;
	uint16_t s_nzones;
	uint16_t s_imap_blocks;
	uint16_t s_zmap_blocks;
	uint16_t s_firstdatazone;
	uint16_t s_log_zone_size;
	uint32_t s_max_size;
	uint16_t s_magic;
	uint16_t s_state;
	uint32_t s_zones;
} __attribute__((packed));

struct minix_dir_entry {
	uint16_t inode;
	char name[0];
} __attribute__((packed));

/* Memory structrures */
struct super_block {
	struct minix_super_block s_sb;

	dev_t	s_dev;
};

struct inode {
	struct minix_inode i_inode;

	struct super_block *i_sb;
};

int read_imap(struct super_block *sb);
void free_imap(struct super_block *sb);
struct inode *get_inode(struct super_block *sb, unsigned int ino);
int read_zmap(struct super_block *sb);
void free_zmap(struct super_block *sb);
struct fs_node *mk_fs_node(struct inode *inode, struct node_operations *ops);

#endif /* MINIX_FS_H */

