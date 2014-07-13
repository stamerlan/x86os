/* File: super.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef SUPER_H
#define SUPER_H

#include <x86os/types.h>

void mount_root(dev_t root_dev);
int sys_mount(char *dev_name, char *dir_name, char *type);

#endif /* SUPER_H */

