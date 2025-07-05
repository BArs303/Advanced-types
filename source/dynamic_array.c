#include "dynamic_array.h"

static void darray_expand(Darray *a);
static void shift_right(Darray *a, unsigned int begin, unsigned int offset);
static void shift_left(Darray *a, unsigned int index, unsigned int offset);
static bool check_darray_index(Darray *a, unsigned int index);

Darray *init_darray_with_length(unsigned int size)
{
	Darray *a;

	a = malloc(sizeof(Darray));
	a->array = malloc(sizeof(void*)*size);
	memset(a->array, 0, sizeof(void*) * size);
	a->size = 0;
	a->capacity = size;
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
	//unsigned int index always >= 0
	return index < a->capacity;
}

void delete_darray
(
	Darray *a,
	void (*free_element)(void *element, void *params),
	void *parameters
)
{
	unsigned int i;
	for(i = 0; i < a->size; i++)
	{
		if(a->array[i])
		{
			free_element(a->array[i], parameters);
		}
	}
	free(a->array);
	free(a);
	return;
}

void darray_clear(Darray *a, void (*free_element)(void *element, void *params), void *params)
{
	unsigned int i;
	for(i = 0; i < a->size; i++)
	{
		free_element(a->array[i], params);
		a->array[i] = NULL;
	}
	a->size = 0;
	return;
}
bool darray_append(Darray *a, void *element)
{
	if(a->size == a->capacity)
		darray_expand(a);
	return darray_insert(a, element, a->size);
}

bool darray_insert(Darray *a, void *element, unsigned int index)
{
	bool result;

	if(check_darray_index(a, index))
	{
		a->array[index] = element;
		index++;
		if(index > a->size)
			a->size = index;
		result = true;
	}
	else
	{
		result = false;
		fprintf(stderr,"Darray invalid index %u\n", index);
	}
	return result;
}

bool darray_delete
(
	Darray *a,
	unsigned int index,
	void (*free_element)(void *element, void *params),
	void *parameters
)
{
	if(check_darray_index(a, index))
	{
		free_element(a->array[index], parameters);
		shift_left(a, index, 1);
		return true;
	}
	fprintf(stderr,"Darray invalid index %u\n", index);
	return false;
}

void print_darray
(
	Darray *a,
	void (*print_element)(void *element, void *params),
	void *parameters
)
{
	unsigned int i;
	for(i = 0; i < a->size; i++)
	{
		printf("darray index: %d\n", i);
		if(a->array[i]) //if element exists
		{
			print_element(a->array[i], parameters);
		}
	}
}

static void darray_expand(Darray *a)
{
	a->array = realloc(a->array, sizeof(void*) * (a->capacity + DEFAULT_DARRAY_STEP_SIZE));
	a->capacity += DEFAULT_DARRAY_STEP_SIZE;
}

static void shift_right(Darray *a, unsigned int start, unsigned int offset)
{
	unsigned int i;
	for(i = a->size + offset - 1; i > start + offset; i--)
	{
		a->array[i] = a->array[i-offset];
	}
}

bool darray_shift_right(Darray *a, unsigned int start, unsigned int offset)
{
	if(check_darray_index(a, a->size + offset) && check_darray_index(a, start))
	{
		shift_right(a, start, offset);
		return true;
	}
	return false;
}

static void shift_left(Darray *a, unsigned int index, unsigned int offset)
{
	unsigned int i;
	for(i = index; i < a->size - offset; i++)
	{
		a->array[i] = a->array[i+offset];
	}
	a->size -= offset;
}

bool darray_shift_left(Darray *a, unsigned int index, unsigned int offset)
{
	if(a->size > offset && index < a->size)
	{
		shift_left(a, index, offset);
		return true;
	}
	return false;
}

bool darray_replace
(
	Darray *a,
	void *new_element,
	unsigned int index,
	void (*free_element)(void *element, void *params),
	void *parameters
)
{
	if(check_darray_index(a, index))
	{
		free_element(a->array[index], parameters);
		a->array[index] = new_element;
		return true;
	}
	return false;
}
