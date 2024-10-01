#include "red_black_tree.h"

static void balance_tree
(
	struct red_black_tree *a,
	struct tree_node *new_node,
	int dir
);

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
struct tree_node* find_node
(
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

struct tree_node* tree_insert
(
	struct red_black_tree *a, 
	void *element,
	int (*compare)(void *arg1, void *arg2)
)
{
	struct tree_node *p, *c;
	int dir;
	dir = LEFT;
	p = find_node(a->root, element, compare);
	c = create_node(element);
	c->parent = p;
	if(p && compare(p->data, element) < 0)
	{
		dir = RIGHT;
	}
	balance_tree(a, c, dir);
	a->root->color = BLACK;
	return c;
}

struct tree_node* rotate_node
(
	struct red_black_tree *a,
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
		a->root = p;
	return p;
}

static void  balance_tree
(
	struct red_black_tree *a,
	struct tree_node *new_node,
	int dir
)
{
	//parent grandparent and uncle of new node
	struct tree_node *p, *g, *u; 	
	p = new_node->parent;
	//tree is empty
	if(!p)
	{
		new_node->color = BLACK;
		a->root = new_node;
		return;
	}
	p->child[dir] = new_node;
	do
	{
		/*
		* case 1
		* parent is black
		* insertion ends
		*/
		if(p->color == BLACK)
			return;
		if(!(g = p->parent))
		{
			//situation when p->parent is null and p is red may occur after deletion
			p->color = BLACK;
			return;
		}
			
		//parent is red. Define parent direction
		dir = g->child[RIGHT] == p?RIGHT:LEFT;
		u = g->child[1-dir];
		//case 2: uncle is black
		if(u == NULL || u->color == BLACK)
			goto case_2;
		/*case 3: uncle is red.
			Recolor the parent and uncle to black 
			Recolor grandparent to red
			Move up the tree to check for further violations
		*/
		p->color = BLACK;
		u->color = BLACK;
		g->color = RED;
		new_node = g;
	}while((p = new_node->parent));
	return;
case_2:
	/*
		if the nodes are arranged in the form of a triangle
		left->right or right->left 
		make it straight line
	*/
	if(new_node == p->child[1-dir])
	{
		rotate_node(a, new_node, dir);
		new_node = p;
		p = g->child[dir];
	}
	/*
		then rotate parent to fix the violations
		p becomes the root of the subtree
		g becomes the child of p
	*/
	rotate_node(a, p, 1-dir);
	p->color = BLACK;
	g->color = RED;
	return;
}
