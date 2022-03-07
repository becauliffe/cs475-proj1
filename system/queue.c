/*
 ============================================================================
 Name        : Queue.c
 Authors     : Madison Sanchez-Forman and Ben Mcaullife
 Version     : 3.7.22
 Description : Defines functions from queue.h to implement Unix ready queue
 ============================================================================
 */
/* queue.c - enqueue, dequeue, isempty, nonempty, et al. */

#include <xinu.h>
#include <stdlib.h>

/**
 * Prints out contents of a queue
 * @param q	pointer to a queue
 */
void printqueue(struct queue *q)
{
	struct qentry *current = q->head;
	kprintf("[");
	while (current != NULL)
	{
		kprintf("(pid= %d), ", current->id);
		current = current->next;
	}
	kprintf("]");
	printf("\n");
}

/**
 * Checks whether queue is empty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8 isempty(struct queue *q)
{

	if (q->size == 0)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
 * Checks whether queue is nonempty
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8 nonempty(struct queue *q)
{
	// TODO - don't just check q's size because q could be NULL
	if (q->size == 0 || q->head == NULL)
	{
		return FALSE;
	}
	else
	{
		return TRUE;
	}
}

/**
 * Checks whether queue is full
 * @param q	Pointer to a queue
 * @return TRUE if true, FALSE otherwise
 */
bool8 isfull(struct queue *q)
{
	// TODO - check if there are at least NPROC processes in the queue
	if (q->size == NPROC)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/**
 * Insert a process at the tail of a queue
 * @param pid	ID process to insert
 * @param q	Pointer to the queue to use
 *
 * @return pid on success, SYSERR otherwise
 */
pid32 enqueue(pid32 pid, struct queue *q)
{
	if (isbadpid(pid) || isfull(q))
	{
		return SYSERR;
	}
	else
	{
		struct qentry *node = (struct qentry *)malloc(sizeof(struct qentry));
		node->id = pid; /*set up new entry*/
		node->next = NULL;
		node->prev = q->tail;
		if (q->head == NULL) /*if q has just been initiated*/
		{
			q->head = node;
			q->size = 1;
		}
		else /*else q is nonempty*/
		{
			q->tail->next = node;
			q->size += 1;
		}
		q->tail = node;
		return node->id;
	}
}

/**
 * Remove and return the first process on a list
 * @param q	Pointer to the queue to use
 * @return pid of the process removed, or EMPTY if queue is empty
 */
pid32 dequeue(struct queue *q)
{
	// TODO - return EMPTY if queue is empty
	if (isempty(q))
	{
		return EMPTY;
	}
	else
	{
		pid32 temp_p = q->head->id;
		struct qentry *temp = q->head; /*store head's pid*/
		if (q->size == 1)
		{
			q->head = NULL; /*if only item on list*/
			q->tail = NULL;
		}
		else
		{
			q->head = q->head->next;
			q->head->prev = NULL;
		}
		q->size -= 1;

		free(temp, sizeof(temp));
		return temp_p;
	}
}

/**
 * Finds a queue entry given pid
 * @param pid	a process ID
 * @param q	a pointer to a queue
 * @return pointer to the entry if found, NULL otherwise
 */
struct qentry *getbypid(pid32 pid, struct queue *q)
{ // TODO - return NULL if queue is empty or if an illegal pid is given
	if (isbadpid(pid) || isempty(q))
	{
		return NULL;
	}
	else
	{ // TODO - find the qentry with the given pid
		struct qentry *current = q->head;
		while (current != NULL)
		{
			if (current->id == pid)
			{ // TODO - return a pointer to it
				return current;
			}
			current = current->next;
		}
		return NULL;
	}
}

/**
 * Remove a process from the front of a queue (pid assumed valid with no check)
 * @param q	pointer to a queue
 * @return pid on success, EMPTY if queue is empty
 */
pid32 getfirst(struct queue *q)
{
	if (isempty(q))
	{
		return EMPTY;
	}
	else
	{ // TODO - remove process from head of queue and return its pid
		pid32 temp = q->head->id;
		dequeue(q);
		return temp;
	}
}

/**
 * Remove a process from the end of a queue (pid assumed valid with no check)
 * @param q	Pointer to the queue
 * @return pid on success, EMPTY otherwise
 */
pid32 getlast(struct queue *q)
{
	// TODO - return EMPTY if queue is empty
	if (isempty(q))
	{
		return EMPTY;
	}
	// TODO - remove process from tail of queue and return its pid
	struct qentry *last = q->tail;
	pid32 temp_pid = last->id;
	q->tail = last->next;
	q->tail->prev = NULL;

	free(last, sizeof(last));
	return temp_pid;
}

/**
 * Remove a process from an arbitrary point in a queue
 * @param pid	ID of process to remove
 * @param q	Pointer to the queue
 * @return pid on success, SYSERR if pid is not found
 */
pid32 remove(pid32 pid, struct queue *q)
{
	// TODO - return EMPTY if queue is empty
	if (isempty(q))
	{
		return EMPTY;
	}
	// TODO - return SYSERR if pid is illegal
	else if (isbadpid(pid))
	{
		return SYSERR;
	}
	struct qentry *current = q->head;
	while (current != NULL)
	{
		// TODO - remove process identified by pid parameter from the queue and return its pid

		if (current->id == pid)
		{
			struct qentry *eNext = current->next;
			struct qentry *ePrev = current->prev;
			eNext->prev = ePrev;
			ePrev->next = eNext;
			pid32 tempId = current->id;
			free(current, sizeof(current));
			return tempId;
		}

		current = current->next;
	}
	// TODO - if pid does not exist in the queue, return SYSERRs
	return SYSERR;
}
