#ifndef SET_H
#define SET_H

#include "red_black_tree.h"

typedef struct red_black_tree Set;

Set* init_set();

bool set_insert(Set *a, void *element, int (*compare)(void *a, void *b));
bool set_delete
(
	Set *a, 
	void *element,
	int (*compare)(void *a, void *b),
	void (*free_element)(void *element)
);

void* set_find(Set *a, void *element, int (*compare)(void *a, void *b));
void delete_set(Set *a, void (*free_element)(void *element));
void print_set(Set *a, void (*print_element)(void *element));

#if 0
 List* set_to_list(Set *a);
#endif

#endif
