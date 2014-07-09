/* File: test.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * Test fs driver
 */

#include <x86os/fs/filesystems.h>
#include <x86os/log.h>

static int read_super(struct super_block *s)
{
	log_printf("testfs: mount\n");

	s->s_blocksize = 512;

	return 0;
}

static struct file_system_type
{
	.read_super = &read_super,
	.name = "testfs",		
}

void init_testfs()
{
	register_filesystem_type(&testfs);
}

