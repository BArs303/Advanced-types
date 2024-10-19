#include "advanced_types.h"

#ifndef DLINKED_LIST_H
#define DLINKED_LIST_H

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
	list_node *head;
	list_node *tail;
	int size;

	list_node *last_used;
	int last_index;
};

List* init_list();
void delete_list(List *a, void (*free_element)(void *element));

void list_append(List *dst, void *element);
void list_insert(List *dst, void *element, int index);
void list_delete(List *a, int index, void (*free_element)(void *element));
void* list_at_pos(List *a, int index);

void print_list(List *a, void (*print_element)(void *element));
void debug_print_list(List *a);

#endif
