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

static char *imap;
static char *zmap;
static unsigned int ninodes;
static unsigned int nzones;
static sector_t inode_table;

static void print_inode(dev_t dev, unsigned int i_no)
{
	struct buf *buf;
	sector_t i_block = i_no / MINIX_INODES_PER_BLOCK;
	struct buf *b = bread(dev, inode_table + i_block * 2);
	log_printf("sector: %d ", inode_table + i_block * 2);
	if (!b) {
		log_printf("can't read inode\n");
		return;
	}
	struct minix_inode *inode = &((struct minix_inode *)&b->data)
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
				(struct minix_dir_entry*)&buf->data;
			for (i = 0; i < 512 / 32; 
				i++, dir = (struct minix_dir_entry*)((char*)dir + 32)) {
				log_printf("   %d (0x%x): inode = %d, name = %s\n",
					i, (char*)dir - buf->data, dir->inode, dir->name);
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
			log_printf("   %s\n", buf->data);
			brelease(buf);
		}

	}

	brelease(b);
}

static struct fs_node *
get_root(dev_t dev)
{
	size_t i;
	struct buf *b, *sb_buf;
	struct minix_super_block *sb;

	// Read sb
	log_printf("minixfs: get_root on %d\n", dev);

	sb_buf = bread(dev, 2);
	if (!sb_buf) {
		log_printf("minixfs: bread failed\n");
		return NULL;
	}

	sb = (struct minix_super_block *)&sb_buf->data;
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

	ninodes = sb->s_ninodes;
	nzones = sb->s_zones;

	// Read imap and zm
	// ap
	imap = kmalloc(sb->s_imap_blocks * BLOCK_SIZE);
	if (!imap) {
		log_printf("minixfs: failed to allocate imap\n");
		return NULL;
	}
	zmap = kmalloc(sb->s_zmap_blocks * BLOCK_SIZE);
	if (!zmap) {
		log_printf("minixfs: failed to allocate zmap\n");
		// kfree imap
		return NULL;
	}

	for (i = 0; i < sb->s_imap_blocks * 2; i++) {
		b = bread(dev, 4 + i);
		log_printf("minixfs: read imap from: %d\n", 4 + i);
		if (!b) {
			log_printf("minixfs: failed to read imap\n");
			// kfree zmap
			// kfree imap
			return NULL;
		}
		memmove(imap + i * 512, b->data, 512);
		brelease(b);
	}
	for (i = 0; i < sb->s_zmap_blocks * 2; i++) {
		b = bread(dev, 4 + sb->s_imap_blocks * 2 + i);
		log_printf("minixfs: read zmap from: %d\n", 
				4 + sb->s_imap_blocks * 2 + i);
		if (!b) {
			log_printf("minixfs: failed to read zmap\n");
			// kfree zmap
			// kfree imap
			return NULL;
		}
		memmove(zmap + i * 512, b->data, 512);
		brelease(b);
	}
	inode_table = 4 + sb->s_imap_blocks * 2 + sb->s_zmap_blocks * 2;
	log_printf("minixfs: inode_table at %d sector\n", inode_table);

	// DEBUG:
	for (i = 0; i < ninodes; i++)
		if (imap[i / 8] & (1 << (i % 8))) {
			log_printf("minixfs: inode %d allocated: ", i);
			print_inode(dev, i);
		}
	for (i = 0; i < nzones; i++)
		if (zmap[i / 8] & (1 << (i % 8)))
			log_printf("minixfs: zone %d allocated\n", i);

	return NULL;
}


struct file_system_type minixfs = {
	.name = "minixfs",
	.get_root = &get_root,
};
