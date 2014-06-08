/*!
 * \file mm.c
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * \brief memory manager
 */

#include "mm.h"
#include "asm.h"

static struct segdesc_t gdt[NR_SEGS];

/// \todo use kmalloc instead
static struct pde_t kpde[1024] __attribute__((aligned(PAGE_SZ)));
static struct pte_t kpte[1024][1024] __attribute__((aligned(PAGE_SZ)));

void mm_init()
{
	// Setup segments
	gdt[SEG_KCODE] = SEG(STA_X | STA_R, 0, 0xFFFFFFFF, DPL_SYS);
	gdt[SEG_KDATA] = SEG(STA_W, 0, 0xFFFFFFFF, DPL_SYS);
	gdt[SEG_UCODE] = SEG(STA_X | STA_R, 0, 0xFFFFFFFF, DPL_USR);
	gdt[SEG_UCODE] = SEG(STA_W, 0, 0xFFFFFFFF, DPL_USR);

	lgdt(gdt, sizeof(gdt));
	set_cs(SEG_KCODE << 3);
	set_ds(SEG_KDATA << 3);
	set_gs(SEG_KDATA << 3);
	set_es(SEG_KDATA << 3);
	set_fs(SEG_KDATA << 3);
	set_ss(SEG_KDATA << 3);

	// Setup virtual memory
	size_t i, j;
	void *p = 0;

	for (i = 0; i < 1024; i++)
	{
		kpde[i] = PDE(&kpte[i]);
	}
	for (i = 0; i < 1024; i++)
		for (j = 0; j < 1024; j++, p += PAGE_SZ)
			kpte[i][j] = PTE(p);

	uint32_t cr3 = (uint32_t)(&kpde) & (~0xFFF);
	write_cr3(cr3);
	uint32_t cr0 = read_cr0();
	cr0 |= 0x80000000;
	write_cr0(cr0);

	/// \todo REMOVE ME. A test map 0x200000 to 0xb8000
	kpte[0][0x200] = PTE(0xb8000);

	char* text = (char*)(0x200000 + 80 * 2 * 3);
	static char text_to_show[] = "Memory mapping is working!";
	for(i = 0; i < sizeof(text_to_show); i++, text += 2)
		*text = text_to_show[i];
}

