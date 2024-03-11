#include "set.h"

static bool check_existence(Set *a, void *element);
static int sort_criteria(void *arg1, void *arg2);

Set *init_set()
{
	return init_darray();
}

Set *init_set_with_length(int length)
{
	return init_darray_with_length(length);
}

void destruct_set(Set *a, void (*free_element)(void* element))
{
	destruct_darray(a, free_element);
}

void print_set(Set *a, void (*print_element)(void* element))
{
	print_darray(a, print_element);
}

void set_clear(Set *a, void (*free_element)(void* element))
{
	darray_clear(a, free_element);
}

bool set_insert(Set *a, void *element)
{
	return s_set_insert(a, element, &sort_criteria);
}

bool s_set_insert(Set *a, void *element, int (*f)(void *arg1, void *arg2))
{
	int i;
	if(check_existence(a, element))
	{
		for(i = 0; i < a->size && f(element, darray_at_pos(a, i)) > 0 ; i++)
		{
		}
		if(i == a->size)
			return darray_append(a, element);
		else
			return darray_insert(a, element, i);		
	}
	return false;

}

bool set_remove(Set *a, int index, void (*free_element)(void* element))
{
	return darray_remove(a, index, free_element);
}

void* set_at_pos(Set *a, int index)
{
	return darray_at_pos(a, index);
}

static bool check_existence(Set *a, void *element)
{
	for(int i = 0; i < a->size; i++)
	{
		if(darray_at_pos(a, i) == element)
		{
			return false;
		}
	}
	return true;
}

static int sort_criteria(void *arg1, void *arg2)
{
	printf("%ld\n", arg1-arg2);
	return arg1 - arg2;
}	
