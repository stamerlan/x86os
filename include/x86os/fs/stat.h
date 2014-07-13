/* File: stat.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef STAT_H
#define STAT_H

#define	S_IFMT		15
#define S_IFDIR		1
#define S_IFFILE	2
#define S_IFCHR		4
#define S_IFBLK		8

#define S_ISDIR(m)	((m & S_IFMT) == S_IFDIR)
#define S_ISFILE(m)	((m & S_IFMT) == S_IFFILE)
#define S_ISCHR(m)	((m & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)	((m & S_IFMT) == S_IFBLK)

#endif /* STAT_H */

