#include "rbt_set.h"


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
	deleted_node = rbt_find_node(a->root, element, compare);
	if(deleted_node && compare(deleted_node->data, element) == 0)
	{
		rbt_delete(a, deleted_node, free_element);
		return true;
	}
	return false;
}

void print_set(Set *a, void (*print_element)(void *element))
{

}

void delete_set(Set *a, void (*free_element)(void *element))
{
	
}
