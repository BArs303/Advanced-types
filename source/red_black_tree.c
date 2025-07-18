#include "red_black_tree.h"

static RB_Node* rbt_max(RB_Node *root);
static void swap_node_colors(RB_Node *a, RB_Node *b);

static RB_Node* rotate_node
(
	struct red_black_tree *a,
	RB_Node *p,
	int dir
);

static void balance_red_black_tree
(
	struct red_black_tree *a,
	RB_Node *new_node,
	int dir
);

static void fixup
(
	RBT *a,
	RB_Node *x
);

static void inner_print_tree(RB_Node *a, void (*print_element)(void *element, void *params), void *params);
static int rbt_cn(RB_Node *a);
static void delete_rbt_node(RB_Node *a, void (*free_element)(void *element, void *params), void *params);

RB_Node* create_red_black_node(void *data)
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

RB_Node* rbt_find_node
(
	RB_Node *root, 
	void *element,
	int (*compare)(void *arg1, void *arg2),
	int *cmp_result
)
{
	RB_Node *parent;
	if(!root)
		return root;

	while(root)
	{
		parent = root;
		*cmp_result = compare(root->data, element);
		if(*cmp_result == 0)
			return root;
		if(*cmp_result > 0)
			root = root->child[LEFT];
		else
			root = root->child[RIGHT];
	}
	return parent;
}

RB_Node* rbt_insert
(
	struct red_black_tree *a, 
	void *element,
	int (*compare)(void *arg1, void *arg2)
)
{
	return s_rbt_insert(a, element, compare, false);
}

RB_Node* s_rbt_insert
(
	struct red_black_tree *a, 
	void *element,
	int (*compare)(void *arg1, void *arg2),
	bool flag
)
{
	RB_Node *p, *c;
	int dir, s;
	dir = LEFT;
	p = rbt_find_node(a->root, element, compare, &s);
	//printf("in insert %d\nfind result %p\n", s, p);
	if(p)
	{
		if(flag && s == 0)//element already exists
			return NULL;
		if(s < 0)
			dir = RIGHT;
	}
	c = create_red_black_node(element);
	c->parent = p;
	balance_red_black_tree(a, c, dir);
	//a->root->color = BLACK;
	return c;
}

static RB_Node* rotate_node
(
	struct red_black_tree *a,
	RB_Node *p,
	int dir
)
{
	RB_Node *g, *m, *root; //grandparent, child and middle
	
	g = p->parent;
	root = g->parent;
	m = p->child[dir];

	//middle element just change its parent
	g->child[1-dir] = m;
	if(m)
		m->parent = g;
	/*two main elements
		p becomes parent for g
	*/
	p->child[dir] = g;
	g->parent = p;

	//root of the subtree
	p->parent = root;
	if(root)
		root->child[root->child[RIGHT] == g ? RIGHT:LEFT] = p;
	else
		a->root = p;
	return p;
}

static void  balance_red_black_tree
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
			//sometimes root may be red. need to figure out
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

void print_rbt(RBT *a, void (*print_element)(void *element, void *params), void *params)
{
	inner_print_tree(a->root, print_element, params);
}
static void inner_print_tree
(
	RB_Node *a,
	void (*print_element)(void *element, void *params),
	void *params
)
{
	if(a)
	{
		inner_print_tree(a->child[LEFT], print_element, params);
		print_element(a->data, params);
		inner_print_tree(a->child[RIGHT], print_element, params);
	}
}


void debug_print_rbt
(
	RB_Node *a,
	void (*print_element)(void *element, void *params),
	void *params
)
{
	if(a)
	{
		printf("value: ");
		print_element(a->data, params);
		printf("color: ");
		if(a->color == RED)
			printf("Red\n");
		else
			printf("Black\n");
		printf("address %p\nparent %p\nleft child\n", a, a->parent);
		debug_print_rbt(a->child[LEFT], print_element, params);
		printf("right child\n");
		debug_print_rbt(a->child[RIGHT], print_element, params);
	}
	else
		printf("nil\n");
}

static void swap_node_colors(RB_Node *a, RB_Node *b)
{
	enum node_color tmp;
	if(a == NULL && b == NULL)
		return;
	else if(a == NULL)
		b->color = BLACK;
	else if(b == NULL)
		a->color = BLACK;
	else
	{
		tmp = a->color;
		a->color = b->color;
		b->color = tmp;
	}
	return;
}
static void fixup
(
	RBT *a,
	RB_Node *x
)
{
	RB_Node *p, *c, *s, *d;
	int dir;
	//x - black node with no childs
	p = x->parent;

	if(!p)//x is root
	{
		//printf("LAST ELEMENT\n\n\n");
		a->root = p;
		return;
	}
	//printf("skipped in fixup\n");
	dir = NODE_DIRECTION(x);
	p->child[dir] = NULL;
	goto cycle_start;
do
{
	//skip for first iteration
	dir = NODE_DIRECTION(x);
	p = x->parent;
cycle_start:
	s = p->child[1-dir]; //sibling
	d = s->child[1-dir]; //distand nephew
	
	if(s->color == RED)
		goto case_1;
	//here s color is black

repeat:
	//if distant nephew is red
	if(d && d->color == RED)
		goto case_32;

	//if close nephew is red
	c = s->child[dir]; //close nephew
	if(c && c->color == RED)
		goto case_31;
	//two nephews are black
	if(p->color == RED)
		goto case_2;
	//p, s, c, d are black
	s->color = RED;
	x = p;
}while((p = x->parent));
return;

//case 2 sibling is red -> p is black
case_1:
	c = s->child[dir];
	rotate_node(a, s, dir);
	swap_node_colors(p, s);
	//changes after rotation
	s = c;
	d = s->child[1-dir];
	goto repeat;

//case 1.3 two nephews and s are black and the parent is red
case_2:
	swap_node_colors(p, s);
	return;

//at least one of the nephews is red
//case 1.2 c - red, s - black
case_31:
	swap_node_colors(s, c);
	rotate_node(a, c, 1-dir);//rotate c and s
	//changes after rotation
	d = s;
	s = c;

//case 1.1 d - red, s - black
case_32:
	swap_node_colors(s, p);
	d->color = BLACK;
	rotate_node(a, s, dir);//rotate sibling and parent
	return;

}

void delete_rbt
(
	RBT *a,
	void (*free_element)(void *element, void *params),
	void *params
)
{
	if(a)
	{
		delete_rbt_node(a->root, free_element, params);
		free(a);
	}
}

static void delete_rbt_node
(
	RB_Node *a,
	void (*free_element)(void *element, void *params),
	void *params
)
{
	if(a)
	{
		if(a->child[LEFT])
			delete_rbt_node(a->child[LEFT], free_element, params);
		if(a->child[RIGHT])
			delete_rbt_node(a->child[RIGHT], free_element, params);
		free_element(a->data, params);
		free(a);
	}
}

void rbt_delete
(
	RBT *a,
	RB_Node *x,
	void (*free_element)(void *element, void *params),
	void *params
)
{
	RB_Node *p, *w, *c;
	enum node_color original_color;
	int dir;

	if(x->child[LEFT] && x->child[RIGHT])
	{
		//if node has two childs replace it with
		//maximum of left subtree
		//or
		//minimum of right subtree
		//prefer red node
		//it will always have maximum 1 child
		w = rbt_max(x->child[LEFT]);
		ptr_swap(&(x->data), &(w->data));
		rbt_delete(a, w, free_element, params);
		return;
	}
	else if(x->child[LEFT])
	{
		c = x->child[LEFT];
	}
	else if(x->child[RIGHT])
	{
		c = x->child[RIGHT];
	}
	else
	{
		original_color = x->color;
		//delete red node and fix up black node
		if(original_color == RED)
		{
			dir = NODE_DIRECTION(x);
			p = x->parent;
			p->child[dir] = NULL;
		}
		else
			fixup(a, x);
		free_element(x->data, params);
		free(x);
		return;
	}
	ptr_swap(&(x->data), &(c->data));
	rbt_delete(a, c, free_element, params);
	return;
}

int rbt_count_nodes(RBT *a)
{
	if(!a->root)
		return 0;
	return rbt_cn(a->root);
}

static int rbt_cn(RB_Node *a)
{
	if(a)
	{
		return rbt_cn(a->child[LEFT]) + rbt_cn(a->child[RIGHT]) + 1;
	}
	return 0;
}
