/*!
 * \file i8259.h
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef I8259_H
#define I8259_H

/*!
 * \defgroup IO_PIC
 * \{
 * \breif I/O Addresses of the two PIC
 */
#define IO_PIC1		0x20	// Master PIC
#define IO_PIC2		0xA0	// Slave PIC
/// *}

/// IRQ at whitch slave connects to master
#define IRQ_SLAVE	2

/// IRQ 0 corresponds to int T_IRQ
#define T_IRQ0		0x20

/// \todo add descr
#define IRQ_TIMER	0
#define IRQ_KBD		1

/// \todo add description
void pic_init();
void pic_enable(int irq);

#endif /* I8259_H */

