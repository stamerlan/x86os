/* File: types.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef TYPES_H
#define TYPES_H

#define NULL	0

typedef unsigned long long	uint64_t;
typedef unsigned int		uint32_t;
typedef unsigned short		uint16_t;
typedef unsigned char		uint8_t;

typedef uint32_t		size_t;

/* The type used for indexing onto a disc or disc partition.
 * x86os always considers sectors to be 512 bytes long independently of the
 * devices real block size
 */
typedef uint64_t sector_t;

/* Device number
 * NOTE: 0 - Invalid device number
 */
typedef uint32_t dev_t;

/* Descriptor Privilege Level
 * TODO: Use define instead
 * TODO: Move it to somewhere else
 */
enum dpl_t
{
	DPL_SYS	= 0,		// System segment
	DPL_USR = 3,		// User segment
};

#endif /* TYPES_H */

