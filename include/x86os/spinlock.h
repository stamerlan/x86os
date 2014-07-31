/* File: spinlock.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <x86os/types.h>

struct spinlock {
	uint32_t locked;
};

void spin_lock_init(struct spinlock *lock);
void spin_lock(struct spinlock *lock);
void spin_unlock(struct spinlock *lock);

#define __SPIN_LOCK_INIT_UNLOCKED(name)	{ 0 }
#define __SPIN_LOCK_INIT_LOCKED(name)	{ 1 }

#endif				/* SPINLOCK_H */
