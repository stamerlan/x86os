/*!
 * \file mm.c
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include "kmalloc.h"
#include "page.h"

#define PAGE_SIZE	0x1000

static struct page_dir *kernel_dir;

/// Initialize pagging
void mm_init()
{
	unsigned int i;
	unsigned int j;
	void *p = (void*)0x00;

	kernel_dir = kmalloc(sizeof(struct page_dir));
	for(i = 0; i < 1024; i++)
		kernel_dir->tables[i] = kmalloc(sizeof(struct page_table));
	
	for (i = 0; i < 1024; i++) 
	{
		for (j = 0; j < 1024; j++) {
			map(&(kernel_dir->tables[i]->page[j]), p, 
				PAGE_PRESENT | PAGE_RW);
			p += PAGE_SIZE;
		}
	}
}

/*!
 * \brief map page to address
 * 
 * \param page - page struct to fill
 * \param addr - phys address
 * \param flags - page flags
 */
static void map(struct page *page, void* addr, unsigned int flags)
{
	page->flags = flags & 0x07;
	page->addr = addr >> 12;
}

/*!
 * \brief switch page directory
 * \detailed loads page_dir address in cr3, enables pagging
 * 
 * \param dir - new page directory
 */
static void switch_pg_dir(struct page_dir)
{
	register int cr0;

	asm volatile("mov %0, %%cr3" :: "r"(&page_dir->tables));
	asm volatile("mov %%cr0, %0" : "=r"(cr0));
	cr0 |= 0x80000000;
	asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

