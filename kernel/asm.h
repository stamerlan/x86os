/*!
 * \file asm.h
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * \details routinues for use some x86 registers and io-ports
 */

#ifndef ASM_H
#define ASM_H

#include "types.h"
#include "mm.h"

/*!
 * \brief read byte from port
 * \param port - port number
 * \return read data
 * \note always inline
 */
static uint8_t inb(uint16_t port) __attribute__((always_inline)); 
static inline uint8_t inb(uint16_t port)
{
	uint8_t data;

	asm volatile("in %1, %0" : "=a"(data) : "d"(port));
	return data;
}

/*!
 * \brief write byte to port
 * \param port - port number
 * \param data - data to write
 * \note always inline
 */
static void outb(uint16_t port, uint8_t data) __attribute__((always_inline));
static inline void outb(uint16_t port, uint8_t data)
{
	asm volatile("out %0, %1" : : "a"(data), "d"(port));
}

/*!
 * \brief setup GDTR
 * \param d - pointer to GDT
 * \param sz - size of GDT
 * \note always inline
 */
static void lgdt(struct segdesc_t* d, size_t sz) __attribute__((always_inline));
static inline void lgdt(struct segdesc_t* d, size_t sz)
{
	struct 
	{
		uint16_t size;
		uint32_t pgdt;
	} __attribute__((packed)) gdtr;

	gdtr.size = sz - 1;
	gdtr.pgdt = (uint32_t)d;

	asm volatile("lgdt (%0)" : : "r"(&gdtr));
}

/// Setup code segment selector
#define set_cs(selector) asm volatile("ljmp %0, $0f\n\t" \
		"0:\n\t" : : "i"(selector))
/// Setup data segment selector
#define set_ds(selector) asm volatile("movw %w0, %%ds" : : "r"(selector))
/// Setup GS segment selector
#define set_gs(selector) asm volatile("movw %w0, %%gs" : : "r"(selector))
/// Setup extra segment selector
#define set_es(selector) asm volatile("movw %w0, %%es" : : "r"(selector))
/// Setup FS segment selector
#define set_fs(selector) asm volatile("movw %w0, %%fs" : : "r"(selector))
/// Setup stack segment selector
#define set_ss(selector) asm volatile("movw %w0, %%ss" : : "r"(selector))

/*!
 * \breif Read CR0 register
 * \return CR0 value
 * \note always inline
 */
static uint32_t read_cr0() __attribute__((always_inline));
static inline uint32_t read_cr0()
{
	uint32_t cr0;
	asm volatile("mov %%cr0, %0" : "=r"(cr0));
	return cr0;
}

/*!
 * \breif Setup CR0 register
 * \param cr0 - new CR0 value
 * \note always inline
 */
static void write_cr0(uint32_t cr0) __attribute__((always_inline));
static inline void write_cr0(uint32_t cr0)
{
	asm volatile("mov %0, %%cr0" :: "r"(cr0));
}

/*!
 * \breif Setup CR3 register
 * \param cr3 - new CR3 value
 * \note always inline
 */
static void write_cr3(uint32_t cr3) __attribute__((always_inline));
static inline void write_cr3(uint32_t cr3)
{
	asm volatile("mov %0, %%cr3" :: "r"(cr3));
}

#endif /* ASM_H */
