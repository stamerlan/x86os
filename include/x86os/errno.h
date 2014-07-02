/* File: errno.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef ERRNO_H
#define ERRNO_H

#define	EPERM		1	/* Operation nor permitted */
#define ENOENT		2	/* No such file or directory */
#define ESRCH		3	/* No such process */
#define EIO		4	/* I/O error */
#define EBADF		5	/* Bad file number */
#define EAGAIN		6	/* Try again */
#define ENOMEM		7	/* Out of memory */
#define EACCES		8	/* Permission denied */
#define EFAULT		9	/* Bad address */
#define ENOTBLK		10	/* Block device required */
#define EBUSY		11	/* Device or resource busy */
#define EEXIST		12	/* File exists */
#define ENODEV		13	/* No such device */
#define ENOTDIR		14	/* Not a directory */
#define EISDIR		15	/* Is a directory */
#define EMFILE		16	/* Too many open files */
#define ENFILE		17	/* File table overflow */
#define EFBIG		18	/* File too large */
#define ENOSPC		19	/* No space left on device */
#define ESPIPE		20	/* Illegal seek */
#define EROFS		21	/* Read-only file system */
#define EMLINK		22	/* Too many links */
#define ENAMETOOLONG	23	/* File name too long */
#define ENOSYS		24	/* Function not implemented */
#define ENOTEMPTY	25	/* Directory not empty */
#define ELOOP		26	/* Too many symbolic links encountered */
#define ENOMSG		27	/* No message of desired type */

#endif /* ERRNO_H */

