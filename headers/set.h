#ifndef SET_H
#define SET_H

#include "red_black_tree.h"
#include "stack.h"

typedef struct red_black_tree Set;
typedef struct set_iter SetIterator;

struct set_iter
{
	RB_Node *root;
	Stack *nodes;
};

Set* init_set();
SetIterator* init_set_iterator(Set *base);

bool set_insert(Set *a, void *element, int (*compare)(void *a, void *b));
bool set_delete
(
	Set *a, 
	void *element,
	int (*compare)(void *a, void *b),
	void (*free_element)(void *element, void *parameters),
	void *parameters
);

void* set_find(Set *a, void *element, int (*compare)(void *a, void *b));
void delete_set(Set *a, void (*free_element)(void *element, void *parameters), void *parameters);
void print_set(Set *a, void (*print_element)(void *element, void *parameters), void *parameters);

void* si_next(SetIterator *iterator);
bool si_has_next(SetIterator *iterator);

#endif
