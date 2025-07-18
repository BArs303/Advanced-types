#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#define LEFT 0
#define RIGHT 1
#define NODE_DIRECTION(n) ((n->parent)->child[RIGHT] == n ? RIGHT:LEFT)


#include "utility.h"

typedef struct red_black_node RB_Node;
typedef struct red_black_tree RBT;

enum node_color
{
	RED,
	BLACK
};

struct red_black_node 
{
	void *data;
	RB_Node *child[2];
	RB_Node *parent;
	enum node_color color;
};

struct red_black_tree
{
	RB_Node *root;
};

/*-----Functions-----*/
RB_Node* rbt_find_node
(
	RB_Node *root,
	void *element,
	int (*compare)(void *arg1, void *arg2),
	int *cmp_result
);
RB_Node* rbt_insert
(
	struct red_black_tree *root,
	void *element,
	int (*compare)(void *arg1, void *arg2)
);

RB_Node* s_rbt_insert
(
	struct red_black_tree *root,
	void *element,
	int (*compare)(void *arg1, void *arg2),
	bool flag
);
void rbt_delete
(
	struct red_black_tree *root,
	RB_Node *element,
	void (*free_element)(void *element, void *parameters),
	void *parameters
);
void delete_rbt(
	RBT *a,
	void (*free_element)(void *element, void *parameters),
	void *parameters
);

RB_Node* create_node(void *data);

void print_rbt
(
	RBT *a,
	void (*print_tree)(void *element, void *parameters),
	void *parameters
);

void debug_print_rbt
(
	RB_Node *a,
	void (*print_tree)(void *element, void *parameters),
	void *parameters
);

int rbt_count_nodes(RBT *a);

#endif
