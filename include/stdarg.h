/* File: vsprintf.c
 * Author: Aleksei Karpenko <nwcdragon.h@gmail.com>
 */

#define va_start(v,l) __builtin_va_start(v,l)
#define va_end(v) __builtin_va_end(v)
#define va_arg(v,l) __builtin_va_arg(v,l)
typedef __builtin_va_list va_list;
