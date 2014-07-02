/* File: filesystems.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <fs/fs.h>
#include <spinlock.h>

// TODO: initialization
static struct
{
	struct spinlock lock;
	struct file_system_type *head;
} file_systems;

void register_filsystem_type(struct file_system_type *fs)
{
	acquire(&file_systems.lock);
	fs.next = file_systems.head;
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

