#include "queue.h"

Queue* init_queue()
{
	return init_list();
}
void push(Queue *a, void *element)
{
	list_append(a,element);
}

void* pop(Queue *a)
{
	void c;
	struct node *b;
	if(a->size > 0){
		b = a->head;
		a->head = a->head->next;
		a->head->previous = NULL;
		free(b);
		c = a->head->data;
		a->head->data = NULL;
		a->size--;
		return c;
	}
	return NULL;
}

bool queue_empty(Queue *a)
{
	return a->size == 0;
}
