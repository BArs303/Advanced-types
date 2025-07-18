#ifndef DLINKED_LIST_H
#define DLINKED_LIST_H

#include "utility.h"

typedef struct dlist_node list_node;
typedef struct dlist List;

struct dlist_node
{
	void *data;
	list_node *next;
	list_node *previous;
};

struct dlist
{
	unsigned int size;
	unsigned int last_index;
	list_node *head;
	list_node *tail;
	list_node *last_used;
};

List* init_list();
void delete_list
(
	List *a,
	void (*free_element)(void *element, void *params),
	void *parameters
);

void* list_at_pos(List *a, unsigned int index);
void list_append(List *dst, void *element);
void list_insert(List *dst, void *element, unsigned int index);
bool list_delete
(
	List *a,
	unsigned int index,
	void (*free_element)(void *element, void *params),
	void *parameters
);

void print_list
(
	List *a,
	void (*print_element)(void *element, void *params),
	void *parameters
);
void debug_print_list(List *a);

#endif
