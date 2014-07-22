/* File: inode.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/log.h>
#include <x86os/errno.h>
#include <x86os/types.h>
#include <x86os/string.h>
#include <x86os/block/bio.h>
#include <x86os/mm/mm.h>
#include <x86os/fs/minix/minix.h>

static char *imap;
static unsigned int ninodes;

static sector_t inode_table;

int
read_imap(struct super_block *sb)
{
	size_t i;
	struct buffer *b;

	ninodes = sb->s_sb.s_imap_blocks;

	imap = kmalloc(ninodes * BLOCK_SIZE);
	if (!imap) {
		log_printf("minixfs: failed to allocate imap\n");
		return -ENOMEM;
	}
	for (i = 0; i < ninodes * 2; i++) {
		b = bread(sb->s_dev, 4 + i);
		log_printf("minixfs: read imap from: %d\n", 4 + i);
		if (!b) {
			log_printf("minixfs: failed to read imap\n");
			return -EIO;
		}
		memmove(imap + i * 512, b->b_data, 512);
		brelease(b);
	}

	inode_table = 4 + sb->s_sb.s_imap_blocks * 2 + sb->s_sb.s_zmap_blocks * 2;
	log_printf("minixfs: inode_table at %d sector\n", inode_table);

	return 0;
}

void free_imap(struct super_block *sb)
{
	// kfree imap
	imap = NULL;
	ninodes = 0;
}

struct inode *
get_inode(struct super_block *sb, unsigned int i_no)
{
	log_printf("minixfs: get_inode %d", i_no);

	if (!i_no || i_no > ninodes) {
		log_printf("minixfs: ninodes = %d\n", ninodes);
		return NULL;
	}
	
	sector_t i_block = i_no / MINIX_INODES_PER_BLOCK;
	struct buffer *b = bread(sb->s_dev, inode_table + i_block * 2);
	log_printf("sector: %d\n", inode_table + i_block * 2);
	if (!b) {
		log_printf("minixfs: get_inode: can't read inode\n");
		return NULL;
	}

	struct inode *inode = kmalloc(sizeof(struct inode));
	if (!inode) {
		log_printf("minixfs: get_inode: kmalloc failed\n");
		brelease(b);
		return NULL;
	}

	memmove(&inode->i_inode, &((struct minix_inode *)&b->b_data)
		[i_no % (512 / sizeof(struct minix_inode))], 
		sizeof(struct minix_inode));
	brelease(b);

	inode->i_sb = sb;

	return inode;
}

/* void put_inode(struct super_block *, unsigned int); */

struct fs_node *mk_fs_node(struct inode *inode)
{
	struct fs_node *node = kmalloc(sizeof(struct fs_node));
	if (!node) {
		log_printf("minixfs: mk_fs_node: kmalloc failed\n");
		return NULL;
	}

	node->dev = inode->i_sb->s_dev;
	node->data = (void*)inode;
	node->mounted = NULL;
	node->op = NULL;

	return node;
	
}

/* void put_fs_node(struct fs_node*); */

