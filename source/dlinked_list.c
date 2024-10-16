#include "dlinked_list.h"

static Node* create_list_node();
static Node* get_node_at_index(List *a, int index);
static bool check_index(List *a, int index);

static Node* forward(Node *initial_node, int begin, int end);
static Node* backward(Node *initial_node, int begin, int end);
static Node* choose_shortest_path(List *a, int index, int x, int y, int z);

List *init_list()
{
	List *new_list;
	new_list = malloc(sizeof(List));

	new_list->head = create_list_node();
	new_list->tail = create_list_node();

	new_list->head->next = new_list->tail;
	new_list->tail->previous = new_list->head;
	new_list->size = 0;

	new_list->last_used = NULL;
	new_list->last_index = -1;
	return new_list;
}

static Node *create_list_node()
{
	Node *a;
	a = malloc(sizeof(Node));
	a->data = NULL;
	a->next = NULL;
	a->previous = NULL;
	return a;
}

void list_append(List *dst, void *element)
{
	dst->tail->data = element;
	dst->tail->next = create_list_node();
	dst->tail->next->previous = dst->tail;
	dst->tail = dst->tail->next;
	dst->size++;
	return;
}

void list_insert(List *dst, void *element, int index)
{
	Node *a, *new_node;
	if(check_index(dst, index))
	{
		a = get_node_at_index(dst, index);

		new_node = create_list_node();
		new_node->next = a;
		new_node->previous = a->previous;
		a->previous = new_node;
		new_node->previous->next = new_node;
		new_node->data = element;
		dst->size++;
	}
	return;
}

void print_list(List *a, void (*print_element)(void *element))
{
	for(Node *i = a->head->next; i->next; i = i->next)
	{
		print_element(i->data);
	}
	return;
}

void debug_print(List *a)
{
	printf("List\nHead %p\nTail %p\nLast used %p\nLast index %d\n\n", a->head, a->tail, a->last_used, a->last_index);
	for(Node *i = a->head; i; i = i->next)
	{
		printf("current node address %p\n"
		"data address %p\n"
		"previous node address %p\n"
		"next node address %p\n\n",
		i,
		i->data,
		i->previous,
		i->next);
	}
	printf("End of List\n");
}

void* list_at_pos(List *a, int index)
{
	Node *tmp;
	void *ret = NULL;
	if(check_index(a, index))
	{
		tmp = get_node_at_index(a, index);
		ret = tmp->data;
	}
	return ret;
}

void list_remove(List *a, int index, void (*free_element)(void *element))
{
	Node *deleted;
	if(!check_index(a, index))
		return;

	deleted = get_node_at_index(a, index);
	deleted->previous->next = deleted->next;
	deleted->next->previous = deleted->previous;
	a->size--;
	free_element(deleted->data);
	free(deleted);

	a->last_used = NULL;
	a->last_index = -1;
	return;
}

void destruct_list(List *a, void (*free_element)(void *element))
{
	Node *tmp;
	for(tmp = a->head->next; tmp->next; tmp = tmp->next)
	{
		if(tmp->data)
		{
			free_element(tmp->data);
		}
		free(tmp->previous);
	}
	free(tmp->previous);
	free(tmp);
	free(a);
	return;
}

static Node* get_node_at_index(List *a, int index)
{
	Node *ret;
	int x, y, z;
	
	x = index; //between 0 and index
	y = abs(index - a->last_index); //between current index and last used index
	z = a->size - index -1; //between n and index where n = last index of list

	ret = choose_shortest_path(a, index, x, y, z);

	a->last_used = ret;
	a->last_index = index;
	return ret;
}

static Node* choose_shortest_path(List *a, int index, int x, int y, int z)
{
	Node *ret;
	if(x < y && x < z)
	{
		ret = forward(a->head->next, 0, index);
	}
	else if(x < z && a->last_index >= 0)
	{
		if((index - a->last_index) > 0)
		{
			ret = forward(a->last_used, a->last_index, index);
		}
		else
		{
			ret = backward(a->last_used, a->last_index, index);
		}
	}
	else
	{
		ret = backward(a->tail->previous, a->size-1, index);
	}
	return ret;
}

static Node* forward(Node *initial_node, int begin, int end)
{
	for(int i = begin; i < end; i++)
	{
		initial_node = initial_node->next;
	}
	return initial_node;
}

static Node* backward(Node *initial_node, int begin, int end)
{
	for(int i = begin; i > end; i--)
	{
		initial_node = initial_node->previous;
	}
	return initial_node;
}

static bool check_index(List *a, int index)
{
	if(index < a->size && index >= 0)
	{
		return true;
	}
	return false;
}
