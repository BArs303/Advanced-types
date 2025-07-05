#include "queue.h"

#ifdef DARRAY_QUEUE_H

Queue* init_queue()
{
	Queue *ret;
	ret = malloc(sizeof(Queue));
	ret->top = 0;
	ret->array = init_darray();
	printf("darray queue\n");
	return ret;
}

void queue_push(Queue *queue, void *element)
{
	darray_insert(queue->array, element, queue->array->size);
}

void* queue_pop(Queue *queue)
{
	void *ret;
	unsigned int offset;
	offset = 10;

	ret = darray_at_pos(queue->array, queue->top);
	queue->top++;

	if(queue->top > offset)
	{
		darray_shift_left(queue->array, 0, offset);
		queue->top = queue->top - offset;
	}
	return ret;
}

bool queue_is_empty(Queue *queue)
{
	return queue->array->size == 0;
}

#else

Queue* init_queue()
{
	return init_list();
}

void queue_push(Queue *a, void *element)
{
	list_append(a, element);
}

void* queue_pop(Queue *queue)
{
	void *ret = NULL;
	ret = list_at_pos(queue, 0);
	list_delete(queue, 0, &passive_destruct, NULL);
	return ret;
}

bool queue_is_empty(Queue *queue)
{
	return queue->size == 0;
}
#endif
