/* File: seg.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef SEG_H
#define SEG_H

#include <x86os/types.h>

// Segmens
#define NR_SEGS		6
#define	SEG_KCODE	1
#define SEG_KDATA	2
#define SEG_UCODE	3
#define SEG_UDATA	4
#define SEG_TSS		5

// Segment descriptor
struct segdesc {
	uint32_t lim_15_0 : 16;		// low bits of seg limit
	uint32_t base_15_0 : 16;	// low bits of seg base addr
	uint32_t base_23_16 : 8;	// middle bits of seg base addr
	/* Segment type bits
	 * TODO: use define instead
	 */
	enum seg_type {
		STA_X	= 0x8,		// app executable
		STA_E	= 0x4,		// app expand down (non-exec seg)
		STA_C	= 0x4,		// app conforming code seg (exec seg)
		STA_W	= 0x2,		// app writeable (non-exec seg)
		STA_R	= 0x2,		// app readable (exec seg)
		STA_A	= 0x1,		// app accessed
	
		STS_T32A = 0x9,		// sys avl 32-bit TSS seg
	} type : 4;
	uint32_t s : 1;			// 0 = sys, 1 = app
	enum dpl_t dpl : 2;		// descriptor privilege level
	uint32_t p : 1;			// present
	uint32_t lim_19_16 : 4;		// high bits of seg limit
	uint32_t avl : 1;		// unused
	uint32_t rsv : 1;		// reserved
	uint32_t db : 1;		// 0 = 16-bit seg, 1 = 32-bit seg
	uint32_t g : 1;			// granularity
	uint32_t base_31_24 : 8;	// high bits of seg base addr
};

// Setup 32-bit segment descriptor
#define SEG(type, base, lim, dpl) (struct segdesc) { 		\
       	((lim) >> 12) & 0xFFFF, (uint32_t)(base) & 0xFFFF,	\
	((uint32_t)(base) >> 16) & 0xFF, (type), 1, (dpl), 1,	\
	(uint32_t)(lim) >> 28, 0, 0, 1, 1, (uint32_t)(base) >> 24 }
// Setup 16-bit segment descriptor
#define SEG16(type, base, lim, dpl) (struct segdesc) {	\
	(lim) & 0xFFFF, (uint32_t)(base) & 0xFFFF,		\
	((uint32_t)(base) >> 16) & 0xFF, (type), 1, (dpl), 1,	\
	(uint32_t)(lim) >> 16, 0, 0, 1, 0, (uint32_t)(base) >> 24 }

#endif /* SEG_H */

