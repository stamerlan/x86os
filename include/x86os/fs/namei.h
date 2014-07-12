/* File: namei.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef NAMEI_H
#define NAMEI_H

int open_namei(const char *name, int flag, struct inode **inode);
int namei(const char *name, struct inode **res_inode);

#endif /* NAMEI_H */

