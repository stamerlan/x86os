/*!
 * \file gdt.c
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

/// Gloabal Descriptors Table
static unsigned char gdt[] __attribute__ ((aligned (8))) = 
{
	/* null */
	0, 0, 0, 0, 0, 0, 0, 0, 
	/* Code, DPL = 0, Base = 0, Lim = 4GB */
	0xFF, 0xFF, 0x00, 0x00, 0x00, 0x9A, 0xCF, 0x00,
	/* Data, DPL = 0, Base = 0, Lim = 4GB */
	0xFF, 0xFF, 0x00, 0x00, 0x00, 0x92, 0xCF, 0x00,
};

/// Loads descriptor table register (gdtr)
void seg_init()
{
	volatile struct 
	{
		unsigned short limit;
		void *base;
	} __attribute__((packed)) gdtr;

	gdtr.limit = (sizeof(gdt) - 1);
	gdtr.base = &gdt;

	asm volatile("lgdt 0(,%0,)" :: "a"(&gdtr));
	asm volatile("ljmp $0x08, $0f\n" \
		"0:\n" \
		"movw $0x10, %ax\n\t" \
		"movw %ax, %ds\n\t" \
		"movw %ax, %gs\n\t" \
		"movw %ax, %es\n\t" \
		"movw %ax, %fs\n\t" \
		"movw %ax, %ss\n\t");
}

