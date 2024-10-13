#include "dynamic_array.h"

static void shift_right(Darray *a, int index);
static void shift_left(Darray *a, int index);
static bool check_darray_index(Darray *a, int index);

Darray *init_darray_with_length(int size)
{
	Darray *a;
	a = malloc(sizeof(Darray));
	a->array = malloc(sizeof(void*) * size);
	for(int i = 0; i < size; i++)
	{
		a->array[i] = NULL;
	}
	a->size = 0; //actual elements in darray
	a->capacity = size; //darray maximum capacity
	return a;
}

Darray* init_darray()
{
	return init_darray_with_length(DEFAULT_DARRAY_SIZE);
}

void* darray_at_pos(Darray *a, int index)
{
	if(check_darray_index(a, index))
	{
		return a->array[index];
	}
	return NULL;
}
static bool check_darray_index(Darray *a, int index)
{
	if(index >= 0 && index < a->capacity)
		return true;
	return false;
}

void destruct_darray(Darray *a, void (*free_element)(void *element))
{
	for(int i = 0; i < a->size; i++)
	{
		free_element(a->array[i]);
	}
	free(a->array);
	free(a);
}

void darray_clear(Darray *a, void (*free_element)(void *element))
{
	for(int i = 0; i < a->size; i++)
	{
		free_element(a->array[i]);
		a->array[i] = NULL;
	}
	a->size = 0;
}

bool darray_append(Darray *a, void *element)
{
	shift_right(a, a->size);
	a->array[a->size] = element;
	a->size++;
	return true;
}

bool darray_insert(Darray *a, void *element, int index)
{
	if(check_darray_index(a, index))
	{
		shift_right(a, index);
		a->array[index] = element;
		a->size++;
		return true;
	}
	printf("Invalid index\n");
	return false;
}

bool darray_remove(Darray *a, int index, void (*free_element)(void *element))
{
	if(check_darray_index(a, index))
	{
		free_element(a->array[index]);
		shift_left(a, index);
		a->size--;
		return true;
	}
	printf("Invalid index\n");
	return false;
}

void print_darray(Darray *a, void(*print_element)(void *element))
{
	for(int i = 0; i < a->size; i++)
	{
		if(a->array[i]) //if element exists
			print_element(a->array[i]);
	}
}

static void shift_right(Darray *a, int index)
{
	int step = DEFAULT_STEP_SIZE;
	if(a->size == a->capacity)
	{
		a->array = realloc(a->array, sizeof(void*) * (a->capacity + step));
		a->capacity += step;
	}

	for(int i = a->size; i > index; i--)
	{
		a->array[i] = a->array[i-1];
	}
	return;
}

static void shift_left(Darray *a, int index)
{
	for(int i = index; i < a->size-1; i++)
	{
		a->array[i] = a->array[i+1];
	}
	return;
}

bool darray_replace(Darray *a, int index, void *new_element, void(*free_element)(void *element))
{
	if(check_darray_index(a, index))
	{
		free_element(a->array[index]);
		a->array[index] = new_element;
		return true;
	}
	return false;
}
