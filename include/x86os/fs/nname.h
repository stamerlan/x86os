/* File: nname.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef NNAME_H
#define NNAME_H

#include <x86os/fs/node.h>

struct fs_node *get_node(const char *filename);

#endif				/* NNAME_H */
