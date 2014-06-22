/* File: i8259.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef I8259_H
#define I8259_H

// I/O Addresses of the two PIC
#define IO_PIC1		0x20	// Master PIC
#define IO_PIC2		0xA0	// Slave PIC

// Processor-defined:
#define T_DIVIDE         0      // divide error
#define T_DEBUG          1      // debug exception
#define T_NMI            2      // non-maskable interrupt
#define T_BRKPT          3      // breakpoint
#define T_OFLOW          4      // overflow
#define T_BOUND          5      // bounds check
#define T_ILLOP          6      // illegal opcode
#define T_DEVICE         7      // device not available
#define T_DBLFLT         8      // double fault
// #define T_COPROC      9      // reserved (not used since 486)
#define T_TSS           10      // invalid task switch segment
#define T_SEGNP         11      // segment not present
#define T_STACK         12      // stack exception
#define T_GPFLT         13      // general protection fault
#define T_PGFLT         14      // page fault
// #define T_RES        15      // reserved
#define T_FPERR         16      // floating point error
#define T_ALIGN         17      // aligment check
#define T_MCHK          18      // machine check
#define T_SIMDERR       19      // SIMD floating point error

/// System call trap
#define T_SYSCALL	64

/// IRQ at whitch slave connects to master
#define IRQ_SLAVE	2

/// IRQ0 corresponds to int T_IRQ
#define T_IRQ0		0x20

#define IRQ_TIMER	0
#define IRQ_KBD		1

void pic_init();
void pic_enable(int irq);

#endif /* I8259_H */

