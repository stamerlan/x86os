/* File: stat.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef STAT_H
#define STAT_H

#define	S_IFMT	07000
#define S_IFDIR	01000
#define S_IFCHR	02000
#define S_IFBLK	04000

#define S_ISDIR(m)	((m & S_IFMT) == S_IFDIR)
#define S_ISCHR(m)	((m & S_IFMT) == S_IFCHR)
#define S_ISBLK(m)	((m & S_IFMT) == S_IFBLK)

#endif /* STAT_H */

