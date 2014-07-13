/* File: fs.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef FS_H 
#define FS_H

#include <x86os/types.h>

void mount_root(dev_t dev);
int sys_open(const char *filename, int flags, mode_t mode);
ssize_t sys_read(const char *filename, char *buf, size_t count);
ssize_t sys_write(const char *filename, char *buf, size_t count);
int sys_close(const char *filename);

#endif /* FS_H */

