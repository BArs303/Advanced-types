#include "set.h"

Set* init_set()
{
	Set *result;
	result = malloc(sizeof(Set));
	result->root = NULL;
	return result;
}

SetIterator* init_set_iterator(Set *base)
{
	SetIterator *result;
	result = malloc(sizeof(SetIterator));
	result->root = base->root;
	result->nodes = init_stack();
	return result;
}

void* si_next(SetIterator *a)
{
	RB_Node *result;
	while(a->root)
	{
		stack_push(a->nodes, a->root);
		a->root = a->root->child[LEFT];
	}
	result = stack_pop(a->nodes);
	a->root = result->child[RIGHT];
	return result->data;
}

bool si_has_next(SetIterator *a)
{
	return !stack_is_empty(a->nodes) || a->root != NULL;
}

bool set_insert(Set *a, void *element, int (*compare)(void *a, void *b))
{
	RB_Node *b;
	b = s_rbt_insert(a, element, compare, true);
	return b != NULL;
}
bool set_delete
(
	Set *a,
	void *element,
	int (*compare)(void *a, void *b),
	void (*free_element)(void *element, void *params),
	void *params
)
{
	RB_Node *deleted_node;
	int cmp_result;
	deleted_node = rbt_find_node(a->root, element, compare, &cmp_result);
	if(deleted_node && cmp_result == 0)
	{
		rbt_delete(a, deleted_node, free_element, params);
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

void print_set(Set *a, void (*print_element)(void *element, void *params), void *params)
{
	print_rbt(a, print_element, params);
}

void delete_set(Set *a, void (*free_element)(void *element, void *params), void *params)
{
	delete_rbt(a, free_element, params);
}

