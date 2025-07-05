#ifndef QUEUE_H
#define QUEUE_H

#ifdef DARRAY_QUEUE_H

#include "dynamic_array.h"

struct _queue
{
	Darray *array;
	unsigned int top;
};

typedef struct _queue Queue;

#else

#include "dlinked_list.h"
typedef struct dlist Queue;

#endif

Queue* init_queue();
void queue_push(Queue *a, void *element);
void* queue_pop(Queue *a);
bool queue_is_empty(Queue *a);

#endif
