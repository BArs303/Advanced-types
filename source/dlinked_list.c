#include "dlinked_list.h"

static list_node* create_list_node(void *data);
static list_node* get_node_at_index(List *a, int index);
static bool check_list_index(List *a, int index);

static list_node* forward(list_node *initial_node, int begin, int end);
static list_node* backward(list_node *initial_node, int begin, int end);
static list_node* choose_shortest_path(List *a, int index, int x, int y, int z);

List *init_list()
{
	List *new_list;
	new_list = malloc(sizeof(List));

	new_list->head = create_list_node(NULL);
	new_list->tail = create_list_node(NULL);

	new_list->head->next = new_list->tail;
	new_list->tail->previous = new_list->head;
	new_list->size = 0;

	new_list->last_used = NULL;
	new_list->last_index = -1;
	return new_list;
}

static list_node* create_list_node(void *data)
{
	list_node *a;
	a = malloc(sizeof(list_node));
	a->data = data;
	a->next = NULL;
	a->previous = NULL;
	return a;
}

void list_append(List *a, void *element)
{
	list_node *new_node;

	new_node = create_list_node(NULL);
	new_node->previous = a->tail;

	a->tail->data = element;
	a->tail->next = new_node;
	a->tail = new_node;
	a->size++;
	return;
}

static void list_insert_between(List *a, void *element, int index)
{
	list_node *new_node, *right, *left;

	new_node = create_list_node(element);
	right = get_node_at_index(a, index);
	left = right->previous;

	right->previous = new_node;
	left->next = new_node;
	new_node->next = right;
	new_node->previous = left;

	a->size++;
	return;
}
void list_insert(List *a, void *element, int index)
{
	list_node *new_node, *right, *left;
	if(index == a->size)
	{
		list_append(a, element);
	}
	else if(check_list_index(a, index))
	{
		list_insert_between(a, element, index);
	}
	return;
}

void print_list(List *a, void (*print_element)(void *element))
{
	for(list_node *i = a->head->next; i->next; i = i->next)
	{
		print_element(i->data);
	}
	return;
}

void debug_print_list(List *a)
{
	printf("List\nHead %p\nTail %p\nLast used %p\nLast index %d\n\n", a->head, a->tail, a->last_used, a->last_index);
	for(list_node *i = a->head; i; i = i->next)
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
	list_node *tmp;
	void *ret = NULL;
	if(check_list_index(a, index))
	{
		tmp = get_node_at_index(a, index);
		ret = tmp->data;
	}
	return ret;
}

void list_delete(List *a, int index, void (*free_element)(void *element))
{
	list_node *deleted;
	if(!check_list_index(a, index))
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

void delete_list(List *a, void (*free_element)(void *element))
{
	list_node *tmp;
	for(tmp = a->head->next; tmp->next; tmp = tmp->next)
	{
		if(tmp->data)
		{
			free_element(tmp->data);
		}
		free(tmp->previous);
	}
	free(tmp->previous);//free head
	free(tmp);//free tail
	free(a);
	return;
}

static list_node* get_node_at_index(List *a, int index)
{
	list_node *ret;
	int x, y, z;
	
	x = index; //between 0 and index
	y = abs(index - a->last_index); //between current index and last used index
	z = a->size - index -1; //between n and index where n = last index of list

	ret = choose_shortest_path(a, index, x, y, z);

	a->last_used = ret;
	a->last_index = index;
	return ret;
}

static list_node* choose_shortest_path(List *a, int index, int x, int y, int z)
{
	list_node *ret;
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

static list_node* forward(list_node *initial_node, int begin, int end)
{
	for(int i = begin; i < end; i++)
	{
		initial_node = initial_node->next;
	}
	return initial_node;
}

static list_node* backward(list_node *initial_node, int begin, int end)
{
	for(int i = begin; i > end; i--)
	{
		initial_node = initial_node->previous;
	}
	return initial_node;
}

static bool check_list_index(List *a, int index)
{
	if(index >= 0 && index < a->size)
	{
		return true;
	}
	return false;
}
