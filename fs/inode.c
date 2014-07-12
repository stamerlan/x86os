/* File: inode.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/fs/fs.h>
#include <x86os/spinlock.h>
#include <x86os/log.h>

void iput(struct inode * inode)
{
	if (!inode)
	{
		log_printf("VFS: iput: null pointer to inode\n");
		return;
	}

	/* TODO: We need to use mutex here */
	acquire(&inode->i_lock);

	if (!inode->i_count)
	{
		log_printf("VFS: iput: trying to free free inode\n" \
			"VFS: device %d, inode %d, mode = %d\n",
			inode->i_dev, inode->i_no, inode->i_mode);

		release(&inode->i_lock);
		return;
	}

	inode->i_count--;
	/* TODO: Wake up waters if !(inode->i_count) */
	release(&inode->i_lock);
}

// Returns: inode
struct inode * iget(struct inode * inode)
{
	if (!inode)
	{
		log_printf("VFS: iget: null ptr to inode\n");
		return NULL;
	}

	acquire(&inode->i_lock);
	inode->i_count++;
	release(&inode->i_lock);

	return inode;
}
