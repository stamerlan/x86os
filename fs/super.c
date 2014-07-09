/* File: super.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/fs/fs.h>

// TODO: make list
// TODO: locks
static struct super_block super_blocks[10];

static struct super_block * alloc_super(dev_t dev)
{
	size_t i;

	if (!dev)
		return NULL;

	for (i = 0; i < 10; i++)
		if (!super_blocks[i].s_dev)
		{
			super_blocks[i].s_dev = dev;
			return &super_blocks[i];
		}

	return NULL;
}

static void free_super(dev_t dev)
{
	size_t i;
	if (!dev)
		return;

	for (i = 0; i < 10; i++)
		if (super_blocks[i].s_dev == dev)
		{
			super_blocks[i].s_dev = 0;
			return;
		}
}

static struct super_block * get_super(dev_t dev)
{
	size_t i;
	if (!dev)
		return NULL;

	for (i = 0; i < 10; i++)
		if (super_blocks[i].s_deb == dev)
			return &super_blocks[i];
}

static struct super_block * read_super(dev_t dev, char *fstype)
{
	struct super_block *s;
	struct file_system_type *type;

	if (!dev)
		return NULL;

	s = get_super(dev);
	if (s)
		return s;
	s = alloc_super(dev);
	if (!s)
		return NULL;

	if (!(type = get_fs_type(fstype)))
	{
		log_printf("VFS: on device %d: get_fs_type(%s) failed\n",
				dev, fstype);
		return NULL;
	}

	if (type->read_super)
	{
		if (!type->read_super(s))
		{
			free_super(dev);
			return NULL;
		}
	}

	return s;
}

// TODO: copy all strings from user space to kernel space
int sys_mount(char *dev_name, char *dir_name, char *type)
{
	int error;
	struct inode *inode;
	struct super_block *sb;

	if (!suser())
		return -EPERM;

	error = namei(dev_name, &inode);
	if (error)
		return error;
	if (inode->i_count != 1 || inode->i_mount)
	{
		iput(inode);
		return -EBUSY;
	}
	if (!FS_ISDIR(inode))
	{
		iput(inode);
		return -EPERM;
	}
	sb = read_super(inode->i_dev, type);
	if (!sb)
	{
		iput(inode);
		return -EBUSY;
	}

	sb->s_covered = inode;
	inode->i_mount = sb->s_root;

	// TODO: umount should iput(inode)
	return 0;
}

