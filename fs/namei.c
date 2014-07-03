/* File: namei.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/types.h>
#include <x86os/proc.h>
#include <x86os/errno.h>
#include <x86os/fs/fs.h>

/* Used to check read/write/execute permissions on a file */
int permission(struct inode *inode, int mask)
{
	int mode = inode->i_mode;

	if (suser())
		return 1;

	if (inode->i_op && inode->i_op->permission)
		return inode->i_op->permission(inode, mask);

	return 0;
}

/* Looks up one part of a pathname, using the fs-dependent routinues for it.
 * It also checks for fathers (pseudo-roots, mount-points)
 */
int lookup(struct inode *dir, const char *name, int len, struct inode **result)
{
	struct super_block *sb;
	int perm;

	*result = NULL;
	if (!dir)
		return -ENOENT;

	/* Check permissions before traversing mount-points */
	perm = permission(dir, MAY_EXEC);
	if (len == 2 && name[0] == '.' && name[1] == '.')
	{
		if (dir == current->root)
		{
			*result = dir;
			return 0;
		} 
		else if ((sb = dir->i_sb) && (dir == sb->s_mounted))
		{
			iput(dir);
			dir = iget(sb->s_covered);
			if (!dir)
				return -ENOENT;
		}
	}

	if (!dir->i_op || !dir->i_op->lookup)
	{
		iput(dir);
		return -ENOTDIR;
	}
	if (!perm)
	{
		iput(dir);
		return -EACCES;
	}
	if (!len)
	{
		*result = dir;
		return 0;
	}
	return dir->i_op->lookup(dir, name, len, result);
}

/* Returns the inode of the directory of the specified name) */
static int dir_namei(const char *pathname, size_t *namelen, const char **name, 
	struct inode * base, struct inode **res_inode)
{
	char c;
	const char *thisname;
	int error;
	size_t len;

	*res_inode = NULL;
	if (!base)
		base = iget(current->pwd);	// inc i_count. Returns inode

	if ((c = *pathname) == '/')
	{
		iput(base);
		base = iget(current->root);
		pathname++;
	}
	for(;;)
	{
		thisname = pathname;
		for(len = 0; (c = *(pathname++)) && (c != '/'); len++)
			/* nothing */ ;

		if (!c)
			break;

		iget(base);
		error = lookup(base, thisname, len, &inode);
		if (error)
		{
			iput(base);
			return error;
		}
	}
	if (!base->i_op || !base->i_op->lookup)
	{
		iput(base);
		return -ENOTDIR;
	}

	*res_inode = base;
	*namelen = len;
	*name = thisname;
	return 0;
}

int open_namei(const char *name, int flag, struct inode **inode)
{
	int error;
	struct inode *dir, *inode;
	size_t namelen;
	const char *basename;

	error = dir_namei(name, &namelen, &basename, NULL, &dir);
	if (error)
		return error;

	iget(dir);
	if (flag & O_CREAT)
	{
		acquite(&dir->i_lock);
		error = lookup(dir, basename, namelen, &inode);
		if (!error)
		{
			if (flag & O_EXCL)
			{
				iput(inode);
				error = -EEXIST;
			}
		}
		else if (!permission(dir, MAY_WRITE | MAY_EXEC))
			error = -EACCES;
		else if (!dir->i_op || !dir->i_op->create)
			error = -EACCES;
		else if (IS_RDONLY(dir))
			error = -EROFS;
		else
		{
			iget(dir);
			error = dir->i_op->create(dir, basename, namelen, mode,
					res_inode);
			release(&dir->i_lock);
			iput(dir);
			return error;
		}
		release(&dir->i_lock);
	}
	else
		error = lookup(dir, basename, namelen, &inode);
	if (error)
	{
		iput(dir);
		return error;
	}
}

