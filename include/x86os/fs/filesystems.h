/* File: filesystems.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef FILESYSTEMS_H
#define FILESYSTEMS_H

#include <x86os/fs/fs.h>

struct file_system_type * get_fs_type(char *name);
void register_filesystem_type(struct file_system_type *fs);
void unregister_filesystem_type(struct file_system_type *fs);

#endif /* FILESYSTEMS_H */
