/* File: asm.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * Routinues for use some x86 registers and io-ports
 */

#ifndef ASM_H
#define ASM_H

#include <x86os/types.h>
#include <x86os/mm/seg.h>
#include <x86os/trap.h>

static inline uint8_t inb(uint16_t port)
{
	uint8_t data;

	asm volatile ("in %1, %0":"=a" (data):"d"(port));
	return data;
}

static inline void outb(uint16_t port, uint8_t data)
{
	asm volatile ("out %0, %1"::"a" (data), "d"(port));
}

static inline void outw(uint16_t port, uint16_t data)
{
	asm volatile ("out %0, %1"::"a" (data), "d"(port));
}

static inline void lgdt(struct segdesc *d, size_t sz)
{
	volatile struct {
		uint16_t size;
		uint32_t pgdt;
	} __attribute__ ((packed)) gdtr;

	gdtr.size = sz - 1;
	gdtr.pgdt = (uint32_t) d;

	asm volatile ("lgdt (%0)"::"r" (&gdtr));
}

static inline void lidt(struct gatedesc *d, size_t sz)
{
	volatile struct {
		uint16_t size;
		uint32_t pidt;
	} __attribute__ ((packed)) idtr;
	idtr.size = sz - 1;
	idtr.pidt = (uint32_t) d;

	asm volatile ("lidt (%0)"::"r" (&idtr));
}

// NOTE: Shouldn't be used directly. Use pushcli() instead
static inline void cli()
{
	asm volatile ("cli");
}

// NOTE: Shouldn't be used directly. Use popcli() instead
static inline void sti()
{
	asm volatile ("sti");
}

#define set_cs(selector) \
	asm volatile("ljmp %0, $0f\n\t" \
		"0:\n\t" : : "i"(selector))
#define set_ds(selector) \
	asm volatile("movw %w0, %%ds" : : "r"(selector))
#define set_gs(selector) \
	asm volatile("movw %w0, %%gs" : : "r"(selector))
#define set_es(selector) \
	asm volatile("movw %w0, %%es" : : "r"(selector))
#define set_fs(selector) \
	asm volatile("movw %w0, %%fs" : : "r"(selector))
#define set_ss(selector) \
	asm volatile("movw %w0, %%ss" : : "r"(selector))

static inline uint32_t rcr0()
{
	uint32_t cr0;
	asm volatile ("mov %%cr0, %0":"=r" (cr0));
	return cr0;
}

static inline void wcr0(uint32_t cr0)
{
	asm volatile ("mov %0, %%cr0"::"r" (cr0));
}

static inline void wcr3(uint32_t cr3)
{
	asm volatile ("mov %0, %%cr3"::"r" (cr3));
}

static inline void ltr(uint16_t sel)
{
	asm volatile ("ltr %0"::"r" (sel));
}

static inline uint32_t xchg(volatile uint32_t * addr, uint32_t newval)
{
	uint32_t result;

	asm volatile ("lock; xchgl %0, %1":"+m" (*addr), "=a"(result):
		      "1"(newval):"cc");

	return result;
}

#endif				/* ASM_H */
