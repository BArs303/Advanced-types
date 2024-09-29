#include "red_black_tree.h"

struct tree_node* create_node(void *data)
{
	struct tree_node *a;
	a = malloc(sizeof(struct tree_node));
	a->data = data;
	a->parent = NULL;
	a->color = RED;
	a->child[LEFT] = NULL;
	a->child[RIGHT] = NULL;
	return a;
}
struct tree_node* find_node(
	struct tree_node *root, 
	void *element,
	int (*compare)(void *arg1, void *arg2)
)
{
	struct tree_node *parent;
	if(!root)
		return root;

	while(root)
	{
		parent = root;
		if(compare(root->data, element) == 0)
			return root;
		else if(compare(root->data, element) > 0)
			root = root->child[LEFT];
		else
			root = root->child[RIGHT];
	}
	return parent;
}

struct tree_node* tree_insert(
	struct tree_node *root, 
	void *element,
	int (*compare)(void *arg1, void *arg2)
)
{
	struct tree_node *a;
	a = find_node(root, element, compare);
	if(a)
	{
		if(compare(a->data, element) > 0)
		{
			a->child[LEFT] = create_node(element);
		}
		else
		{
			a->child[RIGHT] = create_node(element);
		}
		return root;
	}
	else
	{
		root = create_node(element);
	}
	//balance tree
	return root;
}

