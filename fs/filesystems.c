/* File: filesystems.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/fs/fs.h>
#include <x86os/spinlock.h>
#include <x86os/string.h>

// TODO: initialization
static struct
{
	struct spinlock lock;
	struct file_system_type *head;
} file_systems;

struct file_system_type * get_fs_type(char *name)
{
	struct file_system_type *p;

	acquire(&file_systems.lock);
	p = file_systems.head;
	while (p)
	{
		if (!strcmp(name, p->name))
			return p;

		p = p->next;
	}

	return NULL;
}

void register_filesystem_type(struct file_system_type *fs)
{
	acquire(&file_systems.lock);
	fs->next = file_systems.head;
	file_systems.head = fs;
	release(&file_systems.lock);
}

void unregister_filesystem_type(struct file_system_type *fs)
{
	struct file_system_type *p, *prev;

	acquire(&file_systems.lock);
	p = file_systems.head;
	if (p == fs)
	{
		file_systems.head = p->next;
		goto out;
	}

	for(prev = p, p = p->next; p != NULL; prev = p, p = p->next)
	{
		if (p == fs)
		{
			prev->next = p->next;
			goto out;
		}
	}
out:
	release(&file_systems.lock);
}

