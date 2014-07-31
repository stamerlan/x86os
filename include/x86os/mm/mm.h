/* File: mm.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef MM_H
#define MM_H

#include <x86os/types.h>
#include <x86os/proc.h>
#include <x86os/mm/page.h>

void mm_init();
void *kmalloc(size_t sz);
void *kpagealloc(size_t pages);
struct pde *setupvm();
void kmap(struct pde *pde, void *phys, void *virt);
void switchvm(struct task_struct *p);
void switchkvm();

#endif				/* MM_H */
