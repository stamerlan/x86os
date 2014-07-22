/* File: zone.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/log.h>
#include <x86os/errno.h>
#include <x86os/types.h>
#include <x86os/string.h>
#include <x86os/block/bio.h>
#include <x86os/mm/mm.h>
#include <x86os/fs/minix/minix.h>

static char *zmap;
static unsigned int nzones;

int
read_zmap(struct super_block *sb)
{
	size_t i;
	struct buffer *b;

	nzones = sb->s_sb.s_zmap_blocks;

	zmap = kmalloc(nzones * BLOCK_SIZE);
	if (!zmap) {
		log_printf("minixfs: failed to allocate zmap\n");
		return -ENOMEM;
	}

	for (i = 0; i < nzones * 2; i++) {
		b = bread(sb->s_dev, 4 + sb->s_sb.s_imap_blocks * 2 + i);
		log_printf("minixfs: read zmap from: %d\n", 
				4 + sb->s_sb.s_imap_blocks * 2 + i);
		if (!b) {
			log_printf("minixfs: failed to read zmap\n");
			// kfree zmap
			return -EIO;
		}
		memmove(zmap + i * 512, b->b_data, 512);
		brelease(b);
	}

	return 0;
}

void
free_zmap(struct super_block *sb)
{
	// kfree zmap
	zmap = NULL;
	nzones = 0;
}

