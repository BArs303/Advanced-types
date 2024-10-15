#include "advanced_types.h"

#ifndef QUEUE_H
#define QUEUE_H

typedef struct dlist Queue;

Queue* init_queue();
void push(Queue *a, void *element);
void* pop(Queue *a);
bool queue_empty(Queue *a);

#endif
