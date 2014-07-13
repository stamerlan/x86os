/* File: test.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * Test fs driver
 */

#include <x86os/log.h>
#include <x86os/string.h>
#include <x86os/errno.h>
#include <x86os/fs/fs.h>
#include <x86os/fs/stat.h>
#include <x86os/fs/filesystems.h>

static int read_super(struct super_block *s);
static int lookup(struct inode *dir, const char *name, size_t namelen,
		struct inode **result);
static int permission(struct inode *ino, int mask);

static struct file_operations file_op =
{
};

static struct inode_operations dir_op =
{
	.lookup = &lookup,
	.permission = &permission,
};

static struct file_system_type testfs =
{
	.read_super = &read_super,
	.name = "testfs",		
};

/*
 * fs[0] - root directory '/'
 * fs[1] - directory 'bin'
 * fs[2] - file "test"
 */
#define INODE_COUNT 3
static struct inode fs[INODE_COUNT] = 
{ { 1, 0, 0, 512, 0, MAY_READ | MAY_EXEC | MAY_WRITE, S_IFDIR, 0, {0}, 
	&fs[0], &dir_op, NULL, NULL},
  { 2, 0, 0, 512, 0, MAY_READ | MAY_EXEC | MAY_WRITE, S_IFDIR, 0, {0},
	&fs[0], &dir_op, NULL, NULL},
  { 3, 0, 512, 512, 1, MAY_READ | MAY_EXEC | MAY_WRITE, S_IFFILE, 0, {0},
	&fs[0], NULL, &file_op, NULL},
};

static int lookup(struct inode *dir, const char *name, size_t namelen,
		struct inode **result)
{
	size_t i;
	int error;

	log_printf("testfs: lookup: name = ");
	for(i = 0; i < namelen; i++)
		log_printf("%c", name[i]);
	log_printf(" dir = %d\n", dir->i_no);

	error = -ENOENT;
	result = NULL;
	switch (dir->i_no)
	{
	case 1:
		if (namelen != sizeof("bin") - 1)
			break;
		if (strncmp(name, "bin", namelen))
			break;
		*result = &fs[1];
		error = 0;
		break;
	case 2:
		if (namelen != sizeof("test") - 1)
			break;
		if (strncmp(name, "test", namelen))
			break;
		*result = &fs[2];
		error = 0;
		break;
	case 3:
		log_printf("testfs: error lookup for non-directory inode\n");
		break;
	}

	return error;
}

static int permission(struct inode *ino, int mask)
{
	log_printf("testfs: permission ino = %d, mask = %x\n", ino->i_no, mask);

	if (ino->i_no > INODE_COUNT || ino->i_no == 0)
		return 0;

	int i_mode = fs[ino->i_no - 1].i_mode;

	return !(mask ^ i_mode);
}

static int read_super(struct super_block *s)
{
	size_t i;

	log_printf("testfs: mount\n");

	s->s_blocksize = 512;

	for(i = 0; i < INODE_COUNT; i++)
	{
		fs[i].i_dev = s->s_dev;
		fs[i].i_sb = s;
	}

	return 0;
}

void init_testfs()
{
	log_printf("testfs: init\n");
	register_filesystem_type(&testfs);
}

