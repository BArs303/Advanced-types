#include "rbt_set.h"

static void stol(RB_Node *a, List *b);

Set* init_set()
{
	Set *a;
	a = malloc(sizeof(Set));
	a->root = NULL;
	return a;
}

bool set_insert(Set *a, void *element, int (*compare)(void *a, void *b))
{
	RB_Node *b;
	b = s_rbt_insert(a, element, compare, true);
	return b == NULL ? false:true;
}
bool set_delete
(
	Set *a,
	void *element,
	int (*compare)(void *a, void *b),
	void (*free_element)(void *element)
)
{
	RB_Node *deleted_node;
	int cmp_result;
	deleted_node = rbt_find_node(a->root, element, compare, &cmp_result);
	if(deleted_node && cmp_result == 0)
	{
		rbt_delete(a, deleted_node, free_element);
		return true;
	}
	return false;
}
void* set_find(Set *a, void *element, int (*compare)(void *a, void *b))
{
	RB_Node *t;
	int s;
	t = rbt_find_node(a->root, element, compare, &s);
	if(t && s == 0)
		return t->data;
	return NULL;
}

void print_set(Set *a, void (*print_element)(void *element))
{
	print_rbt(a, print_element);
}

void delete_set(Set *a, void (*free_element)(void *element))
{
	
}

List* set_to_list(Set *a)
{
	List *b;
	b = init_list();
	stol(a->root, b);
	return b;
}

static void stol(RB_Node *a, List *b)
{
	if(a)
	{
		stol(a->child[LEFT], b);
		list_append(b, a->data);
		stol(a->child[RIGHT], b);
	}
	return;
}
