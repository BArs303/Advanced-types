#ifndef AT_STACK_H
#define AT_STACK_H


#ifdef DARRAY_STACK_H

#include "dynamic_array.h"
typedef Darray Stack;

#else

#include "dlinked_list.h"
typedef List Stack;

#endif

Stack* init_stack();
void* stack_pop(Stack *a);
void stack_push(Stack *a, void *element);
bool stack_is_empty(Stack *a);


#endif
