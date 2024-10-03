#include "red_black_tree.h"

static RB_Node* rbt_max(RB_Node *root);

static void balance_tree
(
	struct red_black_tree *a,
	RB_Node *new_node,
	int dir
);

RB_Node* create_node(void *data)
{
	RB_Node *a;
	a = malloc(sizeof(RB_Node));
	a->data = data;
	a->parent = NULL;
	a->color = RED;
	a->child[LEFT]= NULL;
	a->child[RIGHT]= NULL;
	return a;
}
RB_Node* find_node
(
	RB_Node *root, 
	void *element,
	int (*compare)(void *arg1, void *arg2)
)
{
	RB_Node *parent;
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

RB_Node* tree_insert
(
	struct red_black_tree *a, 
	void *element,
	int (*compare)(void *arg1, void *arg2)
)
{
	RB_Node *p, *c;
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

RB_Node* rotate_node
(
	struct red_black_tree *a,
	RB_Node *p,
	int dir
)
{
	RB_Node *g, *c, *m, *root; //grandparent, child and middle
	
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
		root->child[root->child[RIGHT] == g ? RIGHT:LEFT] = p;
	else
		a->root = p;
	return p;
}

static void  balance_tree
(
	struct red_black_tree *a,
	RB_Node *new_node,
	int dir
)
{
	//parent grandparent and uncle of new node
	RB_Node *p, *g, *u; 	
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
			//situation when p->parent is null and p is red may occur after rotation
			//sometimes root may be red
			p->color = BLACK;
			return;
		}
			
		//parent is red. Define parent direction
		dir = NODE_DIRECTION(p);
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

static RB_Node* rbt_max(RB_Node *root)
{
	if(root)
	{
		while(root->child[RIGHT])
		{
			root = root->child[RIGHT];
		}
	}
	return root;
}

void bst_delete
(
	struct red_black_tree *a,
	RB_Node *n,
	void (*free_element)(void *element)
)
{
	RB_Node *p;
	RB_Node *t;
	int dir;

	p = n->parent;
	printf("start deletion\n");
	if(!p) //n is root
	{
		a->root = p;
		return;
	}
	dir = NODE_DIRECTION(n);

	if(n->child[LEFT] == NULL && n->child[RIGHT] == NULL)
	{
		p->child[dir] = NULL;
	}
	else if(n->child[LEFT] == NULL)
	{
		printf("delete left\n");
		p->child[dir] = n->child[RIGHT];
		n->child[RIGHT]->parent = p;
	}
	else if(n->child[RIGHT] == NULL)
	{
		printf("delete right\n");
		p->child[dir] = n->child[LEFT];
		n->child[LEFT]->parent = p;
	}
	else
	{
		//if node has 2 childs
		t = rbt_max(n->child[LEFT]);
		ptr_swap(&(n->data), &(t->data));
		bst_delete(a, t, free_element);
		return;
	}
	free_element(n->data);
	free(n);
	return;
}

void print_tree(RB_Node *a)
{
	if(a)
	{
		printf("value %d\ncolor: ", *CAST(int*, a->data));
		if(a->color == RED)
			printf("Red\n");
		else
			printf("Black\n");
		printf("address %p\nparent %p\nleft child\n", a, a->parent);
		print_tree(a->child[LEFT]);
		printf("right child\n");
		print_tree(a->child[RIGHT]);
	}
	else
		printf("nil\n");
}
