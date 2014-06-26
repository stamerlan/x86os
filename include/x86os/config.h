/* File: config.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef CONFIG_H
#define CONFIG_H

/* Kernel stack size
 * NOTE: must be multiple of PAGE_SZ
 */
#define KSTACK_SZ	0x4000

/* Disk cache size (in sectors)
 * NOTE: sector is 512 bytes long
 */
#define BUF_NR		10

#endif /* CONFIG_H */

