/* newqueue.c - newqueue */

#include <xinu.h>
#include <stdlib.h>

/**
 * Allocate and initialize a queue
 * @return pointer to queue structure
 */
struct queue *newqueue(void)
{
	struct queue *ref = (struct queue *)malloc(sizeof(struct queue));
	if (ref == NULL)
	{
		return NULL;
	}
	ref->head = NULL;
	ref->tail = NULL;
	ref->size = 0;
	return ref;
}
