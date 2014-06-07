/*!
 * \file io.h
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * \breif macro to work with IO ports
 * \todo use inline functions
 */

#ifndef IO_H
#define IO_H

/*!
 * \brief Writes byte to port
 *
 * \param port - port number
 * \param value- data to write
 */
#define outpb(port, value) \
	asm volatile("outb %1, %0" :: "dN"((short)(port)), "a"((char)(value)))

/*!
 * \brief Reads byte from port
 *
 * \param port - port number
 * \param value - byte read from port
 */
#define inpb(port, value) \
	asm volatile("inb %1, %0" : "=a"(value) : "dN"((short)(port)));

#endif /* IO_H */

