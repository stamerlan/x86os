/* File: memory.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/asm.h>
#include <x86os/log.h>
#include <x86os/config.h>
#include <x86os/proc.h>
#include <x86os/x86.h>
#include <x86os/mm/seg.h>
#include <x86os/mm/page.h>
#include <x86os/mm/mm.h>
#include <multiboot.h>

static struct segdesc gdt[NR_SEGS];
static struct tss_struct ts;

static struct pde *kpde;
static char *free = (char *) 0x200000;
const char *max_mem = (char *) 0x2000000;	// 32 MB

void
mm_init(size_t modules_count, multiboot_module_t* modules)
{
	size_t i;

	// Setup segments
	gdt[SEG_KCODE] = SEG(STA_X | STA_R, 0, 0xFFFFFFFF, DPL_SYS);
	gdt[SEG_KDATA] = SEG(STA_W, 0, 0xFFFFFFFF, DPL_SYS);
	gdt[SEG_UCODE] = SEG(STA_X | STA_R, 0, 0xFFFFFFFF, DPL_USR);
	gdt[SEG_UDATA] = SEG(STA_W, 0, 0xFFFFFFFF, DPL_USR);

	lgdt(gdt, sizeof (gdt));
	set_cs(SEG_KCODE << 3);
	set_ds(SEG_KDATA << 3);
	set_gs(SEG_KDATA << 3);
	set_es(SEG_KDATA << 3);
	set_fs(SEG_KDATA << 3);
	set_ss(SEG_KDATA << 3);

	kpde = setupvm();

	uint32_t cr3 = (uint32_t) (kpde) & (~0xFFF);
	wcr3(cr3);
	uint32_t cr0 = rcr0();
	cr0 |= CR0_PG;
	wcr0(cr0);

	for (i = 0; i < modules_count; i++, modules++) {
		// NOTE: mark memory as allocated to module
		if (free < (char*)modules->mod_end)
			free = (char *)modules->mod_end;
	}
}

void *
kmalloc(size_t sz)
{

	if (free + sz > max_mem) {
		// PANIC!!!
		log_printf("panic: kpagealloc");
		return NULL;
	}

	void *addr = free;
	free += sz;

	return addr;
}

void *
kpagealloc(size_t pages)
{
	char *new_free = (char *)
	    ((uint32_t) (free + (PAGE_SZ - 1)) & ~(PAGE_SZ - 1));
	if (new_free + (pages * PAGE_SZ) > max_mem) {
		// PANIC!!!
		log_printf("panic: kpagealloc");
		return NULL;
	}

	free = new_free + (pages * PAGE_SZ);
	return new_free;
}

// setups VM
struct pde *
setupvm()
{
	size_t i;
	void *phys = 0;

	struct pde *pde = kpagealloc(1);
	struct pte *pte = kpagealloc(1024);

	log_printf("debug: setupvm(): pde = 0x%x, pte = 0x%x\n",
		   (uint32_t) pde, (uint32_t) pte);

	for (i = 0; i < 1024; i++) {
		pde[i] = PDE(&pte[i * 1024]);
	}
	for (i = 0; i < 1024 * 1024; i++, phys += PAGE_SZ)
		kmap(pde, phys, phys);

	return pde;
}

// NOTO: addr should be 4KB-aligned
// TODO: assumed pte placed one after the other 
void
kmap(struct pde *pde, void *phys, void *virt)
{
	struct pte *pte = (struct pte *) (pde->pte << 12);
	pte[(uint32_t) virt >> 12] = PTE(phys);
}

// Switch TSS and page table to correspond to process p.
void
switchvm(struct task_struct *p)
{
	pushcli();
	gdt[SEG_TSS] = SEG16(STS_T32A, &ts,
			     sizeof (struct tss_struct), DPL_SYS);
	gdt[SEG_TSS].s = 0;
	ts.ss0 = SEG_KDATA << 3;
	ts.esp0 = (uint32_t) p->kstack + KSTACK_SZ;
	ltr(SEG_TSS << 3);
	wcr3((uint32_t) p->pgdir);
	popcli();
}

// Switch page table register to the kernel page table
void
switchkvm()
{
	wcr3((uint32_t) kpde);
}
