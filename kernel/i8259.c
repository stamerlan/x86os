/*!
 * \file i8259.c
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * \breif functions to work with PIC(8259A)
 * \todo use APIC instead
 */

#include "io.h"

#define IO_PIC1_CMD	0x20
#define IO_PIC1_DATA	0x21
#define IO_PIC2_CMD	0xA0
#define IO_PIC2_DATA	0xA1

#define IRQ_SLAVE	2	// IRQ where slave connected to master

#define IRQ0		0x20
#define IRQ8		0x28

/// Setup irq vector offset and mask all interrupts
void pic_init()
{
	// Mask all interrupts
	outpb(IO_PIC1_DATA, 0xfb);
	outpb(IO_PIC2_DATA, 0xff);

	// Set up maseter (8259A-1)
	/* ICW1: 0001g0hi
	 *  g: 0 = edge triggering, 1 = level triggering
	 *  h: 0 = cascaded PICs, 1 = master only
	 *  i: 0 = no ICW4, 1 = ICW4 required
	 */
	outpb(IO_PIC1_CMD, 0x11);
	// ICW2: Vector offset
	outpb(IO_PIC1_DATA, IRQ0);
	/* ICW3: (master PIC) bit mask of IR lines connected to slaves
	 *       (slave PIC) 3-bit # of slave's connection to master
	 */
	outpb(IO_PIC1_DATA, 1 << IRQ_SLAVE);
	/* ICW4: 000nbmap
	 *  n: 1 = special fully nested mode
	 *  b: 1 = buffered mode
	 *  m: 0 = slave PIC, 1 = master PIC
	 *    (ignored when b is 0, as the master/slave role can be hardwired)
	 *  a: 1 = Automatic EOI mode
	 *  p: 0 = MCS-80/85 mode, 1 = intel x86 mode
	 */
	outpb(IO_PIC1_DATA, 0x03);

	// Set up slave (8259A-2)
	outpb(IO_PIC2_CMD, 0x11);
	outpb(IO_PIC2_DATA, IRQ8);
	outpb(IO_PIC2_DATA, IRQ_SLAVE);
	// Automatic EOI mode doesn't work on the slave.
	outpb(IO_PIC2_DATA, 0x03);

	/* OCW3: 0ef01prs
	 *  ef: 0x = NOP, 10 = clear specific mask, 11 = set specific mask
	 *   p: 0 = no pollingm 1 = polling mode
	 *  rs: 0x = NOP, 10 = read IRR, 11 = read ISR
	 */
	outpb(IO_PIC1_CMD, 0x68);      // clear specific mask
	outpb(IO_PIC1_CMD, 0x0a);      // read IRR

	outpb(IO_PIC2_CMD, 0x68);
	outpb(IO_PIC2_CMD, 0x0a);
}

