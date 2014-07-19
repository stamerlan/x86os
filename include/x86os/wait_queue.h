/* File: wait_queue.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * TODO: Lock mutex or semaphore instead spinlock
 */

#ifndef WAIT_QUEUE_H
#define WAIT_QUEUE_H

#include <x86os/spinlock.h>
#include <x86os/list.h>
#include <x86os/proc.h>

struct __wait_queue_head {
	struct spinlock	lock;
	struct list_head task_list;
};

struct __wait_queue {
	struct task_struct *task;
	struct list_head task_list;
};

typedef struct __wait_queue_head wait_queue_head_t;

#define __WAIT_QUEUE_HEAD_INITIALIZER(name) {		\
	.lock = __SPIN_LOCK_INIT_UNLOCKED,			\
	.task_list = LIST_HEAD_INIT((name).lock), }

#define DECLARE_WAIT_QUEUE_HEAD(name) \
	struct wait_queue name = __WAIT_QUEUE_HEAD_INITIALIZER(name)

void init_waitqueue_head(wait_queue_head_t *q);
void wait(wait_queue_head_t *q, struct spinlock *lock);
void wakeup(wait_queue_head_t *q);

#endif /* WAIT_QUEUE_H */

