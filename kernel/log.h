/*!
 * \file log.h
 * \author Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * \todo add function description
 */

#ifndef LOG_H
#define LOG_H

/// \see log.c:log_init()
void log_init();
void log_clear();
void log_putc(char c);
void log_puts(char *s);
void log_printf(char *fmt, ...);

#endif /* LOG_H */

