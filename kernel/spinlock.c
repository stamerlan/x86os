/* File: spinlock.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#include <x86os/spinlock.h>
#include <x86os/asm.h>

/* Initialize a spin lock before using it for the first time */
void spin_lock_init(struct spinlock *lock)
{
	lock->locked = 0;
}

/* Acquire a spin lock, spin waiting if it is not available 
 * TODO: is a lot of time addr bus locked?
 */
void
spin_lock(struct spinlock *lock)
{
	while (xchg(&lock->locked, 1) != 0) ;
}

/* Release a spin lock */
void
spin_unlock(struct spinlock *lock)
{
	xchg(&lock->locked, 0);
}
