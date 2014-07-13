/* File: filesystems.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef FILESYSTEMS_H
#define FILESYSTEMS_H

#include <x86os/types.h>
#include <x86os/fs/node.h>

struct file_system_type
{
	char *name;

	struct fs_node *(*get_root)(dev_t dev);
};

#endif /* FILESYSTEMS_H */

