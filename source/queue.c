#include "queue.h"

Queue* init_queue()
{
	return init_list();
}
void queue_push(Queue *a, void *element)
{
	list_append(a, element);
}

void* queue_pop(Queue *a)
{
	void *ret = NULL;
	if(!queue_is_empty(a))
	{
		a->head = a->head->next;
		ret = a->head->data;
		free(a->head->previous);
		a->head->data = NULL;
		a->head->previous = NULL;
		a->size--;
	}
	return ret;
}

bool queue_is_empty(Queue *a)
{
	return a->size == 0;
}
