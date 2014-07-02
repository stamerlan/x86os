/* File: open.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/types.h>
#include <x86os/config.h>
#include <x86os/proc.h>

/* Open and possibly create a file or device.
 *
 * Given a name for a file, open() returns a file descriptor, a small,
 * nonnegative integer for use in subsequent system calls (read, write, lseek,
 * etc.). The file descriptor returned by a successful call will be the
 * lowest-numbered file descriptor not currently open for the process.
 *
 * The file offset is set to the beginning of the file.
 *
 * A call to open() creates a new open file description, an entry in the
 * system-wide table of open files. This entry records the file offset and
 * the file status flags.
 *
 * The argument flags must include one of the following access modes:
 * O_RDONLY, O_WRONLY or O_RDWR. These request openning the file read-only,
 * write-only, or read/write, respectively.
 */
int open(const char *name, int flags, int mode)
{
	struct inode *inode;
	struct file *f;
	size_t fd;
	int error;

	for (fd = 0; fd < NR_OPEN; fd++)
		if (!current->filp[fd])
			break;

	if (fd >= NR_OPEN)
		return -EMFILE;

	f = get_empty_filp();
	if (!f)
		return -ENFILE;

	current->filp[fd] = f;
	f->f_flags = flags;

	error = open_namei(name, flags, mode, &inode);
	if (error)
	{
		current->filp[fd] = NULL;
		put_filp(f);
		return error;
	}
}

