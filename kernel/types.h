/*!
 * \file types.h
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * \details Some types
 */

#ifndef TYPES_H
#define TYPES_H

#define NULL	0

typedef unsigned int	uint32_t;
typedef unsigned short	uint16_t;
typedef unsigned char	uint8_t;

typedef uint32_t	size_t;		/// type contains size in bytes of smth


/*!
 * \enum dpl_t
 * Descriptor Privilege Level
 */
enum dpl_t
{
	DPL_SYS	= 0,		///< System segment
	DPL_USR = 3,		///< User segment
};

#endif /* TYPES_H */

