#include "dynamic_array.h"

static void darray_expand(Darray *a);
static void shift_right(Darray *a, unsigned int begin);
static void shift_left(Darray *a, unsigned int index);
static bool check_darray_index(Darray *a, unsigned int index);

Darray *init_darray_with_length(unsigned int size)
{
	Darray *a;
	size_t bytes_num;

	bytes_num = size * sizeof(void*);
	a = malloc(sizeof(Darray));
	a->array = malloc(bytes_num);
	memset(a->array, 0, bytes_num);
	a->size = size;
	return a;
}

Darray* init_darray()
{
	return init_darray_with_length(DEFAULT_DARRAY_SIZE);
}

void* darray_at_pos(Darray *a, unsigned int index)
{
	if(check_darray_index(a, index))
	{
		return a->array[index];
	}
	return NULL;
}
static bool check_darray_index(Darray *a, unsigned int index)
{
	if(index >= 0 && index < a->size)
		return true;
	return false;
}

void delete_darray(Darray *a, void (*free_element)(void *element))
{
	unsigned int i;
	for(i = 0; i < a->size; i++)
	{
		free_element(a->array[i]);
	}
	free(a->array);
	free(a);
	return;
}

void darray_clear(Darray *a, void (*free_element)(void *element))
{
	unsigned int i;
	for(i = 0; i < a->size; i++)
	{
		free_element(a->array[i]);
		a->array[i] = NULL;
	}
	return;
}

bool darray_insert(Darray *a, void *element, unsigned int index)
{
	if(check_darray_index(a, index) || a->size == index)
	{
		darray_expand(a);
		shift_right(a, index);
		a->array[index] = element;
		return true;
	}
	perror("Darray invalid index\n");
	return false;
}

bool darray_delete(Darray *a, unsigned int index, void (*free_element)(void *element))
{
	if(check_darray_index(a, index))
	{
		free_element(a->array[index]);
		shift_left(a, index);
		a->size--;
		return true;
	}
	perror("Darray invalid index\n");
	return false;
}

void print_darray(Darray *a, void(*print_element)(void *element))
{
	unsigned int i;
	for(i = 0; i < a->size; i++)
	{
		printf("darray index: %d\n", i);
		if(a->array[i]) //if element exists
		{
			print_element(a->array[i]);
		}
	}
	return;
}

static void darray_expand(Darray *a)
{
	a->array = realloc(a->array, sizeof(void*) * (a->size + DEFAULT_STEP_SIZE));
	a->size += DEFAULT_STEP_SIZE;
	return;
}

static void shift_right(Darray *a, unsigned int begin)
{
	unsigned int i;
	for(i = a->size - 1; i > begin; i--)
	{
		a->array[i] = a->array[i-1];
	}
	return;
}

static void shift_left(Darray *a, unsigned int index)
{
	unsigned int i;
	for(i = index; i < a->size-1; i++)
	{
		a->array[i] = a->array[i+1];
	}
	return;
}

bool darray_replace(Darray *a, void *new_element, unsigned int index, void(*free_element)(void *element))
{
	if(check_darray_index(a, index))
	{
		free_element(a->array[index]);
		a->array[index] = new_element;
		return true;
	}
	return false;
}
