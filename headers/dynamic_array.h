#include "advanced_types.h"

#ifndef DYNAMIC_ARRAY_H
#define DYNAMIC_ARRAY_H
#define DEFAULT_STEP_SIZE 1
#define DEFAULT_DARRAY_SIZE 1

typedef struct dynamic_array Darray;

struct dynamic_array
{
	void **array;
	int size;
	int capacity;
};


Darray *init_darray();
Darray *init_darray_with_length(int length);
void delete_darray(Darray *a, void(*free_element)(void *element));
void print_darray(Darray *a, void(*print_element)(void *element));

void darray_clear(Darray *a, void(*free_element)(void *element));
bool darray_append(Darray *a, void *element);
bool darray_insert(Darray *a, void *element, int index);
bool darray_delete(Darray *a, int index, void(*free_element)(void *element));

void* darray_at_pos(Darray *a, int index);
bool darray_replace(Darray *a, void *new_element, int index, void(*free_element)(void *element));
#endif
