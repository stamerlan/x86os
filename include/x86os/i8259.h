/* File: i8259.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef I8259_H
#define I8259_H

/// IRQ at whitch slave connects to master
#define IRQ_SLAVE	2

/// IRQ0 corresponds to int T_IRQ
#define T_IRQ0		0x20

#define IRQ_TIMER	0
#define IRQ_KBD		1

void pic_init();
void pic_enable(int irq);

#endif				/* I8259_H */
