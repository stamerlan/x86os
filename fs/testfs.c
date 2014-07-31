/* File: testfs.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * Test filesystem driver
 */

#include <x86os/printk.h>
#include <x86os/errno.h>
#include <x86os/string.h>
#include <x86os/fs/filesystems.h>

static const char msg[] = "Hello from testfs driver!\n";
static const size_t msg_len = sizeof (msg);
static struct fs_node *lookup(struct fs_node *node, const char *entry);
static ssize_t read(struct fs_node *node, char *buf, size_t count);

static struct node_operations ops = {
	.read = read,
	.lookup = lookup,
};

static struct fs_node fs[2] = {
	{0, (void *) 1, NULL, &ops},
	{0, (void *) 2, NULL, &ops},
};

static ssize_t
read(struct fs_node *node, char *buf, size_t count)
{
	if ((int) node->data != 2)
		return -EFAULT;

	if (count > msg_len)
		count = msg_len;

	memmove(buf, (const void *) &msg, count);
	return count;
}

static struct fs_node *
lookup(struct fs_node *node, const char *entry)
{
	printk("debug: testfs: lookup: node = %d, entry = %s\n",
	       (int) node->data, entry);

	if ((int) node->data != 1)
		return NULL;

	if (!strcmp(entry, "test"))
		return &fs[1];

	return NULL;
}

static struct fs_node *
get_root(dev_t dev)
{
	printk("testfs: get_root on %d\n", dev);

	return &fs[0];
}

struct file_system_type testfs = {
	.name = "testfs",
	.get_root = &get_root,
};
