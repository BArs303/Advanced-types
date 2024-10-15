#include "queue.h"

Queue* init_queue()
{
	return init_list();
}
void push(Queue *a, void *element)
{
	struct node *b;
	a->tail->data = element;

}

void* pop(Queue *a)
{
	
}

bool queue_empty(Queue *a)
{
	
}
