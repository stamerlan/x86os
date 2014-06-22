/* File: page.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef PAGE_H
#define PAGE_H

#include <x86os/types.h>

// Memory page size
#define PAGE_SZ	0x1000

// Page Directory Entry
struct pde
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
	uint32_t avl : 3;	// available
	uint32_t pte : 20;
} __attribute__((packed));

// Setup PDE (present, read/write, user)
#define PDE(PTE_adr) (struct pde) { \
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0, (uint32_t)(PTE_adr) >> 12 }

// Page Table Entry
struct pte
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
	uint32_t avl : 3;	// available
	uint32_t phys : 20;

} __attribute__((packed));

// Setup PTE (present, read/write, user)
#define PTE(PHYS_adr) (struct pte) { \
	1, 1, 1, 0, 0, 0, 0, 0, 0, 0, (uint32_t)(PHYS_adr) >> 12 }

#endif /* PAGE_H */

