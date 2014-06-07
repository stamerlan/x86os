/*!
 * \file page.h
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef PAGE_H
#define PAGE_H

#define PAGE_PRESENT	0x01
#define PAGE_RONLY	0x00
#define PAGE_RW         0x02
#define PAGE_SYSTEM     0x00
#define PAGE_USER       0x04

struct page 
{
	/* flags: 0000 00000uwp
	 *  u: 1 = user 0 = system
	 *  w: 1 = rw   0 = read only
	 *  p: 1 = present
	 */
	unsigned int flags : 12;
	unsigned int phys  : 20;
};

struct page_table 
{
	struct page[1024];
};

struct page_dir 
{
	struct page_table *tables[1024];
}

#endif /* PAGE_H */
