/* File: list.h
 * Author: Vlad Vovchenko <vlad.vovchenko93@gmail.com>
 *
 * Simple doubly linked list implementation
 */

#ifndef LIST_H
#define LIST_H

struct list_head {
	struct list_head *next, *prev;
};

#define LIST_HEAD_INIT(name) { &(name), &(name) }

#define LIST_HEAD(name) \
	struct list_head (name) = LIST_HEAD_INIT(name)

/* Initializes a dynamically allocated list_head
 *
 * NOTE: You can omit this if subsequent call is add_list() or anything that 
 * contains next/prev fields initialization.
 */
static inline void INIT_LIST_HEAD(struct list_head *list)
{
	list->next = list;
	list->prev = list;
}

/* Insert a new entry berween two known consecutive entries
 *
 * NOTE: This is only for internal list manipultation.
 */
static inline void __list_add(struct list_head *new, struct list_head *prev,
		struct list_head *next)
{
	next->prev = new;
	new->next = next;
	new->prev = prev;
	prev->next = new;
}

/* list_add - add a new entry
 * @new:	new entry to be added.
 * @head:	list head to add it after.
 *
 * Insert a new entry after the specified head.
 * NOTE: This is good for implementing stacks.
 */
static inline void list_add(struct list_head *new, struct list_head *head)
{
	__list_add(new, head, head->next);
}

/* list_add_tail - add a new entry
 * @new:	new entry to be added.
 * @head:	list head to add it before.
 *
 * Insert a new entry before the specified head.
 * NOTE: This is useful for implementing queues.
 */
static inline void list_add_tail(struct list_head *new, struct list_head *head)
{
	__list_add(new, head->prev, head);
}

/* Delete a list entry by making the prev/next entries point to each other.
 *
 * NOTE: This is only for internal list manupulation.
 */
static inline void __list_del(struct list_head * prev, struct list_head * next)
{
	next->prev = prev;
	prev->next = next;
}

/* list_del - deletes entry from list.
 * @entry: the element to delete from list.
 */
static inline void list_del(struct list_head *entry)
{
	__list_del(entry->prev, entry->next);

	// TODO: Use special value for removed entries (for debugging)
	entry->next = NULL;
	entry->prev = NULL;
}

#define offsetof(TYPE, MEMBER) ((size_t) &((TYPE *)0)->MEMBER)

/* list_entry - get the struct for this entry
 * @ptr:	the &struct list_head pointer.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the member within the struct.
 */
#define list_entry(ptr, type, member) ({			\
	const typeof( ((type *)0)->member ) *__mptr = (ptr);	\
	(type *)( (char *)__mptr - offsetof(type, member) );})

/* list_first_entry - get the first element from a list
 * @ptr:	the list head to take element from.
 * @type:	the type of the struct this is embedded in.
 * @member:	the name of the list_struct within the struct.
 */
#define list_first_entry(ptr, type, member)	\
	list_entry((ptr)->next, type, member)

/* list_next_entry - get the next element in list
 * @pos:	current element.
 * @member:	the name of the list_struct whithin the struct.
 */
#define list_next_entry(pos, member) \
	list_entry((pos)->member.next, typeof(*(pos)), member)

/* list_for_each - iterate over a list
 * @pos:	the &struct list_head to use as a loop counter.
 * @head:	the head for yourr list.
 */
#define list_for_each(pos, head) \
	for ((pos) = (head)->next; (pos) != (head); (pos) = (pos)->next)

/* list_for_each_entry - iterate over list of given type
 * @pos:	a loop cursor.
 * @head:	the head for your list.
 * @member:	the name of the list_struct within the struct.
 */
#define list_for_each_entry(pos, head, member)				\
	for ((pos) = list_first_entry(head, typeof(*pos), member);	\
	     &((pos)->member) != (head);				\
	     pos = list_next_entry(pos, member))

#endif /* LIST_H */

