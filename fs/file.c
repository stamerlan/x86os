/* File: file.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/types.h>
#include <x86os/spinlock.h>
#include <x86os/mm/mm.h>
#include <x86os/fs/fs.h>

// TODO: add initialization
static struct
{
	struct file *head;
	struct spinlock lock;
} file_table;

struct file * get_empty_filep()
{
	struct file *f = kmalloc(sizeof(struct file));

	if (!f)
		return NULL;

	acquire(&file_table.lock);
	f->f_next = file_table.head;
	f->f_prev = NULL;
	if (file_table.head)
	{
		file_table.head->f_prev = f;
	}
	file_table.head = f;

	f->f_count = 1;

	release(&file_table.lock);

	return f;
}

void put_filp(struct file *f)
{
	struct file *p, *prev;

	acquire(&f->f_lock);
	if (--f->f_count)
	{
		release(&f->f_lock);
		return;
	}

	acquire(&file_table.lock);
	for(p = file_table.head, prev = NULL; p != NULL; 
		prev = p, p = p->f_next)
	{
		if (p == f)
		{
			prev->f_next = f->f_next;
			f->f_next->f_prev = prev;

			// TODO: Don't release, mark as free
			// kfree(f)
			break;
		}
	}
	release(&file_table.lock);
}
