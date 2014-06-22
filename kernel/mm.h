/* File: mm.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef MM_H
#define MM_H

#include "types.h"

// Segmens
#define NR_SEGS		6
#define	SEG_KCODE	1
#define SEG_KDATA	2
#define SEG_UCODE	3
#define SEG_UDATA	4
#define SEG_TSS		5

// Segment descriptor
struct segdesc_t
{
	uint32_t lim_15_0 : 16;		// low bits of seg limit
	uint32_t base_15_0 : 16;	// low bits of seg base addr
	uint32_t base_23_16 : 8;	// middle bits of seg base addr
	/* Segment type bits
	 * TODO: use define instead
	 */
	enum seg_type
	{
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
#define SEG(type, base, lim, dpl) (struct segdesc_t) { 		\
       	((lim) >> 12) & 0xFFFF, (uint32_t)(base) & 0xFFFF,	\
	((uint32_t)(base) >> 16) & 0xFF, (type), 1, (dpl), 1,	\
	(uint32_t)(lim) >> 28, 0, 0, 1, 1, (uint32_t)(base) >> 24 }
// Setup 16-bit segment descriptor
#define SEG16(type, base, lim, dpl) (struct segdesc_t) {	\
	(lim) & 0xFFFF, (uint32_t)(base) & 0xFFFF,		\
	((uint32_t)(base) >> 16) & 0xFF, (type), 1, (dpl), 1,	\
	(uint32_t)(lim) >> 16, 0, 0, 1, 0, (uint32_t)(base) >> 24 }

// Memory page size
#define PAGE_SZ	0x1000

// Page Directory Entry
struct pde_t
{
	uint32_t p : 1;		// present
	uint32_t rw : 1;	// read / write
	uint32_t u : 1;		// usr / sys: 1 = usr, 0 = sys
	uint32_t w : 1;		// write through
	uint32_t d : 1;		// cache disabled
	uint32_t a : 1;		// acessed
	uint32_t rsv : 1;	// reserved
	uint32_t s : 1;		// page sz: 1 = 4 MB, 0 = 4 KB
	uint32_t g : 1;		// ignored
	uint32_t avl : 3;	// availabl
	uint32_t pte : 20;
} __attribute__((packed));

// Setup PDE (present, read/write, user)
#define PDE(PTE_adr) (struct pde_t) { \
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0, (uint32_t)(PTE_adr) >> 12 }

// Page Table Entry
struct pte_t
{
	uint32_t p : 1;		// present
	uint32_t rw : 1;	// read / write
	uint32_t u : 1;		// usr / sys: 1 = usr, 0 = sys
	uint32_t w : 1;		// write through
	uint32_t c : 1;		// cache disabled
	uint32_t a : 1;		// acessed
	uint32_t d : 1;		// dirty
	uint32_t rsv : 1;	// reserved
	uint32_t g : 1;		// global
	uint32_t avl : 3;	// availabl
	uint32_t phys : 20;

} __attribute__((packed));

// Setup PTE (present, read/write, user)
#define PTE(PHYS_adr) (struct pte_t) { \
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0, (uint32_t)(PHYS_adr) >> 12 }

void mm_init();
void *kmalloc(size_t sz);
void *kpagealloc(size_t pages);
struct pde_t *setupvm();
void kmap(struct pde_t *pde, char *phys, char *virt);
void yield();
void sched();

#endif /* MM_H */

