#include "advanced_types.h"

#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#define LEFT 0
#define RIGHT 1

#define NODE_DIRECTION(n) ((n->parent)->child[RIGHT] == n ? RIGHT:LEFT)

typedef struct red_black_node RB_Node;

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

RB_Node* find_node(
	RB_Node *root,
	void *element,
	int (*compare)(void *arg1, void *arg2)
);

RB_Node* tree_insert(
	struct red_black_tree *root,
	void *element,
	int (*compare)(void *arg1, void *arg2)
);

RB_Node* rotate_node(
	struct red_black_tree *t,
	RB_Node *p,
	int dir
);

void bst_delete
(
	struct red_black_tree *root,
	RB_Node *element,
	void (*free_element)(void *element)
);

RB_Node* create_node(void *data);

//temporary functions
void print_tree(RB_Node *a);
#endif
