/* File: vsprintf.h
 * Author: Aleksei Karpenko <nwcdragon.h@gmail.com>
 */

#ifndef VSPRINTF_H
#define VSPRINTF_H

int vsprintf(char *buf, const char *fmt, va_list args);
static int skip_atoi(const char **s);
static char* number(char* str, int num, int base, int size, int type);

#endif
