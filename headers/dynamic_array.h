#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H

#define DEFAULT_DARRAY_STEP_SIZE 10
#define DEFAULT_DARRAY_SIZE 1

#include "utility.h"

typedef struct dynamic_array Darray;

struct dynamic_array
{
	unsigned int size;
	unsigned int capacity;
	void **array;
};


Darray *init_darray();
Darray *init_darray_with_length(unsigned int length);
void delete_darray
(
	Darray *a,
	void(*free_element)(void *element, void *params),
	void *parameters
);

void darray_clear(Darray *a, void(*free_element)(void *element, void *parameters), void *parameters);
bool darray_insert(Darray *a, void *element, unsigned int index);
bool darray_append(Darray *a, void *element);
bool darray_delete
(
	Darray *a,
	unsigned int index,
	void(*free_element)(void *element, void *params),
	void *parameters
);

void print_darray
(
	Darray *a,
	void(*print_element)(void *element, void *params),
	void *params
);

void* darray_at_pos(Darray *a, unsigned int index);
bool darray_replace
(
	Darray *a,
	void *new_element,
	unsigned int index,
	void(*free_element)(void *element, void *params),
	void *paremeters
);
bool darray_shift_left(Darray *array, unsigned int index, unsigned int offset);
bool darray_shift_rigth(Darray *array, unsigned int start, unsigned int offset);

#endif
