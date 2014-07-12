/* File: file.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef FILE_H
#define FILE_H

#include <x86os/fs/fs.h>

struct file * get_empty_filep();
void put_filp(struct file *f);

#endif /* FILE_H */
