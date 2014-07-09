/* File: fcntl.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef FCNTL_H
#define FCNTL_H

/* open flags
 * NOTE: 2 lowest bits is access mode 
 */
#define O_ACCMODE	3
#define O_RDONLY	0
#define O_WRONLY	1
#define O_RDWR		2
#define O_CREAT		4

#endif /* FCNTL_H */

