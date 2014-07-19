/* File: wait_queue.c
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 * TODO: a process may acquire lock, other processes should continue sleeping
 */

#include <x86os/wait_queue.h>
#include <x86os/proc.h>
#include <x86os/spinlock.h>
#include <x86os/mm/mm.h>

void wait(wait_queue_head_t *q, struct spinlock *lock)
{
	struct __wait_queue *w = kmalloc(sizeof(struct __wait_queue));
	if (!w)
	{
		// PANIC!!!
		return;
	}


	spin_lock(&q->lock);
	current->state = TASK_SLEEPING;
	list_add_tail(&q->task_list, &w->task_list);
	w->task = current;
	spin_unlock(lock);
	spin_unlock(&q->lock);

	sched();

	spin_lock(lock);
}

void wakeup(wait_queue_head_t *q)
{
	struct __wait_queue *p;

	list_for_each_entry(p, &q->task_list, task_list)
		p->task->state = TASK_RUNNABLE;
	// TODO: IMPORTANT clear list

	spin_unlock(&q->lock);
}

void init_waitqueue_head(wait_queue_head_t *q)
{
	spin_lock_init(&q->lock);
	INIT_LIST_HEAD(&q->task_list);
}

