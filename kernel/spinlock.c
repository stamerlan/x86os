/* File: spinlock.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/spinlock.h>
#include <x86os/asm.h>

void
acquire(struct spinlock *lock)
{
	while (xchg(&lock->locked, 1) != 0) ;
}

void
release(struct spinlock *lock)
{
	xchg(&lock->locked, 0);
}
