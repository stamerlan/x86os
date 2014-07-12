/* File: namei.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/types.h>
#include <x86os/proc.h>
#include <x86os/errno.h>
#include <x86os/fs/fs.h>
#include <x86os/fs/inode.h>

/* Used to check read/write/execute permissions on a file */
static int permission(struct inode *inode, int mask)
{
	if (suser())
		return 1;

	if (inode->i_op && inode->i_op->permission)
		return inode->i_op->permission(inode, mask);

	return 0;
}

/* Looks up one part of a pathname, using the fs-dependent routinues for it.
 * It also checks for fathers (pseudo-roots, mount-points)
 * NOTE: dir should be got already (iget())
 */
static int lookup(struct inode *dir, const char *name, size_t len, 
		struct inode **result)
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
		else if ((sb = dir->i_sb) && (dir == sb->s_root))
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

/* Returns the inode of the directory of the specified name */
static int dir_namei(const char *pathname, size_t *namelen, const char **name, 
	struct inode **res_inode)
{
	char c;
	const char *thisname;
	int error;
	size_t len;
	struct inode *inode, *base;

	*res_inode = NULL;

	if ((c = *pathname) == '/')
	{
		base = current->root;
		pathname++;
	}
	else
	{
		base = current->pwd;
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
		iput(base);
		if (error)
			return error;
		base = inode;
	}

	if (!base->i_op || !base->i_op->lookup)
		return -ENOTDIR;

	*res_inode = base;
	*namelen = len;
	*name = thisname;
	return 0;
}

// Returns inode of file name (inode already got)
int namei(const char *name, struct inode **res_inode)
{
	int error;
	size_t filenamelen;
	const char *filename;
	struct inode *dir, *inode;

	*res_inode = NULL;

	error = dir_namei(name, &filenamelen, &filename, &dir);
	if (error)
		return error;

	iget(dir);
	error = lookup(dir, filename, filenamelen, &inode);
	iput(dir);
	if (error)
		return error;
	
	iget(inode);
	*res_inode = inode;
	return 0;
}

// NOTE: on releasing inode should be put
int open_namei(const char *name, int flag, struct inode **inode)
{
	int error;
	struct inode *dir;
	size_t namelen;
	const char *basename;

	inode = NULL;

	error = dir_namei(name, &namelen, &basename, &dir);
	if (error)
		return error;

	iget(dir);
	// TODO: flags
	error = lookup(dir, basename, namelen, inode);
	return error;
}

