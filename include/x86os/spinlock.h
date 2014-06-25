/* File: spinlock.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 */

#ifndef SPINLOCK_H
#define SPINLOCK_H

#include <x86os/types.h>

struct spinlock
{
	uint32_t locked;
};

void acquire(struct spinlock *lock);
void release(struct spinlock *lock);

#endif /* SPINLOCK_H */

