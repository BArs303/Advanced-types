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
	struct tree_node *p, *c;
	p = find_node(root, element, compare);
	if(p)
	{
		c = create_node(element);
		c->parent = p;
		if(compare(p->data, element) > 0)
		{
			p->child[LEFT] = c;
		}
		else
		{
			p->child[RIGHT] = c;
		}
	}
	else
	{
		root = create_node(element);
		root->color = BLACK;
	}
	//balance tree
	return root;
}

struct tree_node* rotate_node(
	struct red_black_tree *t,
	struct tree_node *p,
	int dir
)
{
	struct tree_node *g, *c, *m, *root; //grandparent, child and middle
	
	g = p->parent;
	root = g->parent;
	m = p->child[dir];

	//middle element rotation
	g->child[1-dir] = m;
	if(m)
		m->parent = g;
	//two main elements
	p->child[dir] = g;
	g->parent = p;

	p->parent = root;
	if(root)
		root->child[g == root->child[RIGHT]?RIGHT:LEFT] = p;
	else
		t->root = p;

	return p;
}

