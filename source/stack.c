#include "stack.h"

#ifdef DARRAY_STACK_H
Stack* init_stack()
{
	return init_darray_with_length(10);
}

void* stack_pop(Stack *stack)
{
	void *result;
	result = darray_at_pos(stack, stack->size - 1);
	stack->size--;
	return result;
}

void stack_push(Stack *stack, void *element)
{
	darray_insert(stack, element, stack->size);
}

bool stack_is_empty(Stack *stack)
{
	return stack->size == 0;
}

#else

Stack* init_stack()
{
	return init_list();
}

void* stack_pop(Stack *stack)
{
	void *result;
	result = list_at_pos(stack, stack->size-1);
	list_delete(stack, stack->size - 1, &passive_destruct, NULL);
	return result;
}

void stack_push(Stack *a, void *element)
{
	list_append(a, element);
}

bool stack_is_empty(Stack *a)
{
	return a->size == 0;
}

#endif
