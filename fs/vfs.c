/* File: vfs.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * TODO: Check is functions implemented in driver
 */

#include <x86os/types.h>
#include <x86os/errno.h>
#include <x86os/log.h>
#include <x86os/fs/node.h>
#include <x86os/fs/filesystems.h>
#include <x86os/fs/nname.h>

#define NR_FILESYSTEMS	1

extern struct file_system_type testfs;

struct fs_node *root;
static struct file_system_type* filesystems[NR_FILESYSTEMS] = {&testfs};

void mount_root(dev_t root_dev)
{
	size_t i;

	for(i = 0; i < NR_FILESYSTEMS; i++)
	{
		root = filesystems[i]->get_root(root_dev);
		if (root)
		{
			log_printf("vfs: root fs mounted (%s filesystem)\n",
					filesystems[i]->name);
			return;
		}
	}

	// PANIC!!!
	log_printf("panic: vfs: can not mount root\n");
	return;
}

int open(const char *filename, int flags, mode_t mode)
{
	struct fs_node *node = get_node(filename);

	if (!node)
		return -ENOENT;

	return node->op->open(node, flags, mode);
}

ssize_t read(const char *filename, char *buf, size_t count)
{
	struct fs_node *node = get_node(filename);

	if (!node)
		return -ENOENT;

	return node->op->read(node, buf, count);
}

ssize_t write(const char *filename, char *buf, size_t count)
{
	struct fs_node *node = get_node(filename);

	if (!node)
		return -ENOENT;

	return node->op->write(node, buf, count);
}

int close(const char *filename)
{
	struct fs_node *node = get_node(filename);

	if (!node)
		return -ENOENT;

	return node->op->close(node);
}

