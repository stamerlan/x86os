/* File: fs.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef FS_H
#define FS_H

#include <x86os/types.h>
#include <x86os/spinlock.h>

#define MAY_EXEC	1
#define MAY_WRITE	2
#define MAY_READ	4

/* Object super_block must be implemented for each file system. It describes
 * filesystem. It corresponds superblock, which stored in a special disk block.
 * File systems without superblock must generate it in a memory.
 */
struct super_block
{
	dev_t s_dev;
	unsigned long s_blocksize;	// Block size in bytes
	struct inode *s_covered;
	struct inode * s_root;

	struct super_operations *s_op;
	void *s_fs_info;		// For driver use
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
	void (*write_super)(struct super_block *sb);
};

/* Object inode contains all information for manipulating files and directories.
 * If file system have no inodes this information should be obtained from disk
 * structures.
 */
struct inode
{
	unsigned long i_no;
	dev_t i_dev;
	off_t i_size;			// File size in bytes
	unsigned long i_blksize;	// Block size in bytes
	unsigned long i_blocks;		// File size in blocks
	int i_mode;			// Access rights
	int i_type;			// File type

	unsigned int i_count;
	struct spinlock i_lock;

	struct inode *i_mount;		// root of mounted fs

	struct inode_operations	*i_op;
	struct file_operations *i_fop;
	struct super_block *i_sb;
};

struct inode_operations
{
	// Create new inode in directory dir
	int (*create)(struct inode *dir, char *name, int mode);
	// Search for entry in directory dir
	int (*lookup)(struct inode *dir, const char *name, size_t namelen,
			struct inode **result);
	// Create new directory entry in directory dir
	int (*mkdir)(struct inode *dir, char *entry, int mode);
	// Remove directory entry from directory dir
	int (*rmdir)(struct inode *dir, char *entry);
	// Move old_entry from old_dir to new_entry in new_dir
	int (*rename)(struct inode *old_dir, char *old_entry,
			struct inode *new_dir, char *new_entry);
	// Check permission
	int (*permission)(struct inode *ino, int mask); 
};

/* File object is used to represent opened by process files. Processes work with
 * files. Object created on open() syscall and destroyed on close(). File may be
 * opened by some processes, so some file objects may be created for a disk
 * file.
 */
struct file
{
	off_t f_pos;
	int f_flags;
	struct inode *f_inode;

	int f_count;
	struct spinlock f_lock;
	struct file_operations *f_op;
	struct file *f_next, *f_prev;
};

struct file_operations
{
	// Set file pointer in file
	off_t (*lseek)(struct file *file, off_t offset, int origin);
	// Read count bytes from file, update file pos, read data put in buf
	ssize_t (*read)(struct file *file, char *buf, size_t count);
	/* Write count bytes to file, update file pos, 
	 * data to write get from buf
	 */
	ssize_t (*write)(struct file *file, const char *buf, size_t count);
	// Create new file object and link it with inode
	int (*open)(struct inode *inode, struct file *file);
	// Invoked on file close
	int (*release)(struct inode *inode, struct file *file);
};

/* File system type descriptor. Each fs driver have such object */
struct file_system_type
{
	int (*read_super)(struct super_block *);
	char *name;

	struct file_system_type *next;
};

#endif /* FS_H */

