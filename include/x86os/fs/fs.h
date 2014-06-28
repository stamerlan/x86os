/* File: fs.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef FS_H
#define FS_H

#include <x86os/types.h>

/* Object super_block must be implemented for each file system. It describes
 * filesystem. It corresponds superblock, which stored in a special disk block.
 * File systems without superblock must generate it in a memory.
 */
struct super_block
{
	dev_t		s_dev;
	unsigned long	s_blocksize;	// Block size in bytes
	struct dentry	*s_oot;

	struct super_operations *s_op;
};

struct super_operations
{
	// Create and initialize new inode
	struct inode *(*alloc_inode)(struct super_block *sb);
	// Destroy inode
	void (*destroy_inode)(struct inode *inode);
	// Read from disk inode and fill all fields (inode->i_no)
	void (*read_inode)(struct inode *inode);
	// Write inode to disk
	void (*write_inode)(struct inode *inode);

	// Write superblock to disk
	void write_super(struct super_block *sb);

};

/* Object inode contains all information for manipulating files and directories.
 * If file system have no inodes this information should be obtained from disk
 * structures.
 */
struct inode
{
	unsigned long	i_no;
	off_t		i_size;		// File size in bytes
	unsigned long	i_blksize;	// Block size in bytes
	unsigned long	i_blocks;	// File size in blocks
	int		i_mode;		// Access rights
	int		i_type;		// File type

	struct inode_operations	*i_op;
	struct file_operations	*i_fop;
	struct super_block	*i_sb;
};

struct inode_operations
{
	// Create new inode dentry in directory dir
	int (*create)(struct inode *dir, struct dentry *dentry, int mode);
	// Search for dentry in directory dir
	struct dentry *(*lookup)(struct inode *dir, struct dentry *dentry);
	// Create new directory dentry in directory dir
	int (*mkdir)(struct inode *dir, struct dentry *dentry, int mode);
	// Remove directory dentry from directory dir
	int (*rmdir)(struct inode *dir, struct dentry *dentry);
	// Move old_dentry from old_dir to new_dentry in new_dir
	int (*rename)(struct inode *old_dir, struct dentry *old_dentry,
			struct inode *new_dir, struct dentry *new_dentry);
};

/* VFS subsystem represents directories the same way as files. In /bin/vi bin
 * and vi - files, but bin - special file, which is directory, vi - regular
 * file. Inodes used to represent each file, but VFS need some specific
 * operations for directories, like searching by name, existence, etc.
 * To do it VFS have directory entries (dentry). Object dentry - path element.
 * /. bin and vi - directory entries. All path elements (regular and special
 * files) are dentries.
 * Dentries may be moutpoints. Subsytem VFS creates dentry objects.
 * TODO: add info
 */
struct dentry
{
	struct inode			*d_inode;
	struct super_block		*d_sb;
	char 				*d_name;	// TODO: What about hash

	struct dentry_operations	*d_op;

	struct dentry			*d_parent;

};

struct dentry_operations
{
	// Compare 2 filenames
	int (*d_compare)(struct dentry *dentry, char *name1, char *name2);
};

/* File object is used to represent opened by process files. Processes work with
 * files. Object created on open() syscall and destroyed on close(). File may be
 * opened by some processes, so some file objects may be created for a disk
 * file.
 */
struct file
{
	struct dentry		*f_dentry;
	struct file_operations	*f_op;
	
	off_t			f_pos;
};

struct file_operations
{
	// Set file pointer in file
	off_t (*seek)(struct file *file, off_t offset, int origin);
	// Read count bytes from file, update file pos, read data put in buf
	ssize_t (*read)(struct file *file, char *buf, size_t count);
	/* Write count bytes to file, update file pos, 
	 * data to write get from buf
	 */
	ssize_t (*write)(struct file *file, const char *buf, size_t count);
	// Create new file object and link it with inode
	int open(struct inode *inode, struct file *file);
	// Invoked on file close
	int release(struct inode *inode, struct file *file);
};

#endif /* FS_H */

