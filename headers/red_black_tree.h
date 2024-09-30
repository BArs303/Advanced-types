#include "advanced_types.h"

#ifndef RED_BLACK_TREE_H
#define RED_BLACK_TREE_H

#define LEFT 0
#define RIGHT 1

enum node_color
{
	RED,
	BLACK
};

struct tree_node
{
	void *data;
	struct tree_node *child[2];
	struct tree_node *parent;
	enum node_color color;
};

struct red_black_tree
{
	struct tree_node *root;
};

struct tree_node* find_node(
	struct tree_node *root,
	void *element,
	int (*compare)(void *arg1, void *arg2)
);

struct tree_node* tree_insert(
	struct tree_node *root,
	void *element,
	int (*compare)(void *arg1, void *arg2)
);

struct tree_node* rotate_node(
	struct red_black_tree *t,
	struct tree_node *p,
	int dir
);
struct tree_node* create_node(void *data);

#endif
