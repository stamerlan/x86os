/* File: super.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/errno.h>
#include <x86os/log.h>
#include <x86os/proc.h>
#include <x86os/fs/fs.h>
#include <x86os/fs/filesystems.h>
#include <x86os/fs/namei.h>
#include <x86os/fs/inode.h>
#include <x86os/fs/stat.h>

// TODO: Move it some where else
// TODO: Does we realy need it?
struct inode *root_inode;

// TODO: make list
// TODO: locks
static struct super_block super_blocks[10];

static struct super_block * alloc_super(dev_t dev)
{
	size_t i;

	log_printf("debug: alloc_super on %d\n", dev);

	if (!dev)
		return NULL;

	for (i = 0; i < 10; i++)
		if (!super_blocks[i].s_dev)
		{
			log_printf("debug: alloc_super: allocated %d sb\n", i);
			super_blocks[i].s_dev = dev;
			return &super_blocks[i];
		}

	log_printf("debug: alloc_super failed\n");
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
		if (super_blocks[i].s_dev == dev)
			return &super_blocks[i];
	return NULL;
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
		if (type->read_super(s))
		{
			log_printf("debug: VFS: read_super error\n");
			free_super(dev);
			return NULL;
		}
	}

	log_printf("debug: VFS: read_super finish\n");

	return s;
}

// TODO: copy all strings from user space to kernel space
int sys_mount(char *dev_name, char *dir_name, char *type)
{
	int error;
	struct inode *dev_inode, *dir_inode;
	struct super_block *sb;

	if (!suser())
		return -EPERM;

	error = namei(dev_name, &dev_inode);
	if (error)
		return error;
	if (dev_inode->i_count != 1 || dev_inode->i_mount)
	{
		iput(dev_inode);
		return -EBUSY;
	}
	if (!S_ISBLK(dev_inode->i_mode))
	{
		iput(dev_inode);
		return -ENOTBLK;
	}

	error = namei(dir_name, &dir_inode);
	if (error)
	{
		iput(dev_inode);
		return error;
	}
	if (!S_ISDIR(dir_inode->i_mode))
	{
		iput(dir_inode);
		iput(dev_inode);
		return -EPERM;
	}
	sb = read_super(dev_inode->i_dev, type);
	if (!sb)
	{
		iput(dir_inode);
		iput(dev_inode);
		return -EBUSY;
	}

	sb->s_covered = dir_inode;
	dev_inode->i_mount = sb->s_root;

	// TODO: umount should iput(inode)
	return 0;
}

void mount_root(dev_t root_dev)
{
	log_printf("debug: vfs: mount root on %d\n", root_dev);

	struct super_block *sb;
	struct file_system_type *fs_type = get_filesystems();

	while(fs_type != NULL)
	{
		sb = read_super(root_dev, fs_type->name);
		if (sb)
		{
			root_inode = sb->s_root;

			log_printf("VFS: mounted root (%s filesystem)\n",
					fs_type->name);

			return;
		}

		fs_type = fs_type->next;
	}

	// PANIC
	log_printf("vfs: can not mount root\n");
}

