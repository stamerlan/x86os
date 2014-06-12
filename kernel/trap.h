/*!
 * \file trap.h
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef TRAP_H
#define TRAP_H

#include "types.h"

/*!
 * \struct gatedesc_t
 * \breif gate descriptors for interrupts and traps
 */
struct gatedesc_t
{
	uint32_t ofs_15_0 : 16;		///< low 16 bits of offset in segment
	uint32_t cs : 16;		///< code segment selector
	uint32_t args : 5;		///< # args, 0 for int/trap gates
	uint32_t rsv : 3;		///< reserved
	//uint32_t type : 4;
	/*!
	 * \enum gate_type
	 * sysyem segment type bits
	 */
	enum gate_type
	{
		STS_IG32 = 0xE,		///< 32-bit int gate
		STS_TG32 = 0xF,		///< 32-bit trap gate
	} type : 4;
	uint32_t s : 1;			///< must be 0 (system)
	enum dpl_t dpl : 2;		///< descr (meaning new) privilege lvl
	uint32_t p : 1;			///< present
	uint32_t ofs_31_16 : 16;	///< high bits of offset in segment
};

/// \todo add documentation
// Layout of the trap frame built on the stack by the
// hardware and by trapasm.S, and passed to trap().
struct trapframe_t
{
	// pusha
	uint32_t edi;
	uint32_t esi;
	uint32_t ebp;
	uint32_t oesp;
	uint32_t ebx;
	uint32_t edx;
	uint32_t ecx;
	uint32_t eax;

	uint16_t gs;
	uint16_t pad1;
	uint16_t fs;
	uint16_t pad2;
	uint16_t es;
	uint16_t pad3;
	uint16_t ds;
	uint16_t pad4;

	uint32_t trapno;

	// def by hardware
	uint32_t err;
	uint32_t eip;
	uint16_t cs;
	uint16_t pad5;
	uint32_t eflags;

	// only when crossing rings
	uint32_t esp;
	uint16_t ss;
	uint16_t pad6;
};

/*!
 * \def SETGATE
 * \details Set up a normal interrupt/trap gate descriptor.
 * - \a istrap: 1 for a trap (= exception) gate, 0 for an interrupt gate.
 *   interrupt gate clears FL_IF, trap gate leaves FL_IF alone
 * - \a sel: Code segment selector for interrupt/trap handler
 * - \a ofs: Offset in code segment for interrupt/trap handler
 * - \a d: Descriptor Privilege Level -
 *        the privilege level required for software to invoke
 *        this interrupt/trap gate explicitly using an int instruction.
 */
#define SETGATE(gate, istrap, sel, ofs, d)		\
{							\
	  (gate).ofs_15_0 = (uint32_t)(ofs) & 0xffff;	\
	  (gate).cs = (sel);				\
	  (gate).args = 0;				\
	  (gate).rsv = 0;				\
	  (gate).type = (istrap) ? STS_TG32 : STS_IG32;	\
	  (gate).s = 0;					\
	  (gate).dpl = (d);				\
	  (gate).p = 1;					\
	  (gate).ofs_31_16 = (uint32_t)(ofs) >> 16;	\
}

/// \todo add description
void idt_init();

#endif /* TRAP_H */

