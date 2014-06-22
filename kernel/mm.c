/* File: mm.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include "mm.h"
#include "asm.h"
#include "log.h"
#include "x86.h"
#include "proc.h"
#include "config.h"

static struct segdesc_t gdt[NR_SEGS];
static struct taskstate_t ts;
static struct context_t *kcontext;
// TODO: make static
struct pde_t *kpde; 

void swtch(struct context_t **old, struct context_t *new);

static char *free = (char*)0x200000;
const char *max_mem = (char*)0x2000000;	// 32 MB

extern struct proc_t *proc_list;
struct proc_t *current = NULL;

void mm_init()
{
	// Setup segments
	gdt[SEG_KCODE] = SEG(STA_X | STA_R, 0, 0xFFFFFFFF, DPL_SYS);
	gdt[SEG_KDATA] = SEG(STA_W, 0, 0xFFFFFFFF, DPL_SYS);
	gdt[SEG_UCODE] = SEG(STA_X | STA_R, 0, 0xFFFFFFFF, DPL_USR);
	gdt[SEG_UDATA] = SEG(STA_W, 0, 0xFFFFFFFF, DPL_USR);

	lgdt(gdt, sizeof(gdt));
	set_cs(SEG_KCODE << 3);
	set_ds(SEG_KDATA << 3);
	set_gs(SEG_KDATA << 3);
	set_es(SEG_KDATA << 3);
	set_fs(SEG_KDATA << 3);
	set_ss(SEG_KDATA << 3);

	kpde = setupvm();

	uint32_t cr3 = (uint32_t)(kpde) & (~0xFFF);
	wcr3(cr3);
	uint32_t cr0 = rcr0();
	cr0 |= CR0_PG;
	wcr0(cr0);

	/*
	// TODO: REMOVE ME. A test: map 0x1ff000 to 0xb8000
	kmap(kpde, (char*)0xb8000, (char*)0x1ff000);

	char* text = (char*)(0x1ff000 + 80 * 2 * 1);
	static char text_to_show[] = "kmap() works fine!";
	size_t i;
	for(i = 0; i < sizeof(text_to_show); i++, text += 2)
		*text = text_to_show[i];
	*/
}

void *kmalloc(size_t sz)
{

	if (free + sz > max_mem)
	{
		// PANIC!!!
		log_printf("panic: kpagealloc");
		return NULL;
	}

	void *addr = free;
	free += sz;

	return addr;
}

void *kpagealloc(size_t pages)
{
	char *new_free = (char*)
		((uint32_t)(free + (PAGE_SZ - 1)) & ~(PAGE_SZ - 1));
	if (new_free + (pages * PAGE_SZ) > max_mem)
	{
		// PANIC!!!
		log_printf("panic: kpagealloc");
		return NULL;
	}

	free = new_free + (pages * PAGE_SZ);
	return new_free;
}

// setups VM
struct pde_t *setupvm()
{
	size_t i;
	char *phys = 0;

	struct pde_t *pde = kpagealloc(1);
	struct pte_t *pte = kpagealloc(1024);

	log_printf("debug: setupvm(): pde = 0x%x, pte = 0x%x\n",
			(uint32_t)pde, (uint32_t)pte);

	for (i = 0; i < 1024; i++)
	{
		pde[i] = PDE(&pte[i * 1024]);
	}
	for (i = 0; i < 1024 * 1024; i++, phys += PAGE_SZ)
		kmap(pde, phys, phys);

	return pde;
}

// NOTO: addr should be 4KB-aligned
// TODO: assumed pte placed one after the other 
void kmap(struct pde_t *pde, char *phys, char *virt)
{
	struct pte_t *pte = (struct pte_t*)(pde->pte << 12);
	pte[(uint32_t)virt >> 12] = PTE(phys);
}

static void switchvm(struct proc_t *p)
{
	pushcli();
	gdt[SEG_TSS] = SEG16(STS_T32A, &ts,
			sizeof(struct taskstate_t), DPL_SYS);
	gdt[SEG_TSS].s = 0;
	ts.ss0 = SEG_KDATA << 3;
	ts.esp0 = (uint32_t)p->kstack + KSTACK_SZ;
	ltr(SEG_TSS << 3);
	wcr3((uint32_t)p->pgdir);
	popcli();
}

void yield()
{
	swtch(&current->context, kcontext);
}

// only kernel context
void sched()
{
	for(;;)
	{
		struct proc_t *p;
		for(p = proc_list; p != NULL; p = p->next)
		{
			if (p->state != RUNNABLE)
				continue;

			current = p;
			switchvm(p);
			p->state = RUNNING;
			swtch(&kcontext, p->context);
			p->state = RUNNABLE;
		}
	}
}
