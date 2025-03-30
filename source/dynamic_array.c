#include "dynamic_array.h"

static void darray_expand(Darray *a);
static int darray_find_emptiness(Darray *a);
static void shift_right(Darray *a, int begin, int end);

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

void delete_darray(Darray *a, void (*free_element)(void *element))
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
	int end;
	end = darray_find_emptiness(a);
	if(end == a->capacity)
		darray_expand(a);

	a->array[end] = element;
	a->size++;
	return true;
}

bool darray_insert(Darray *a, void *element, int index)
{
	int end;
	if(check_darray_index(a, index))
	{
		end = darray_find_emptiness(a);
		if(end == a->capacity)
			darray_expand(a);
		shift_right(a, index, end);
		a->array[index] = element;
		a->size++;
		return true;
	}
	printf("Invalid index\n");
	return false;
}

bool darray_delete(Darray *a, int index, void (*free_element)(void *element))
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
	int i;
	for(i = 0; i < a->capacity; i++)
	{
		if(a->array[i]) //if element exists
		{
			printf("darray index: %d\n", i);
			print_element(a->array[i]);
		}
	}
}
static int darray_find_emptiness(Darray *a)
{
	int end;
	for(end = a->capacity - 1; end >= 0; end--)
	{
		if(a->array[end])
			break;
	}
	return ++end;
}
static void darray_expand(Darray *a)
{
	a->array = realloc(a->array, sizeof(void*) * (a->capacity + DEFAULT_STEP_SIZE));
	a->capacity += DEFAULT_STEP_SIZE;
	return;
}

static void shift_right(Darray *a, int begin, int end)
{
	int i;
	for(i = end; i > begin; i--)
	{
		a->array[i] = a->array[i-1];
	}
	return;
}

static void shift_left(Darray *a, int index)
{
	int i;
	for(i = index; i < a->size-1; i++)
	{
		a->array[i] = a->array[i+1];
	}
	return;
}

bool darray_replace(Darray *a, void *new_element, int index, void(*free_element)(void *element))
{
	if(check_darray_index(a, index))
	{
		free_element(a->array[index]);
		a->array[index] = new_element;
		return true;
	}
	return false;
}
