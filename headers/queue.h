#include "advanced_types.h"

#ifndef QUEUE_H
#define QUEUE_H

typedef struct dlist Queue;

Queue* init_queue();
void queue_push(Queue *a, void *element);
void* queue_pop(Queue *a);
bool queue_is_empty(Queue *a);

#endif
