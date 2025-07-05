#include "dlinked_list.h"

static list_node* create_list_node(void *data);
static list_node* get_node_at_index(List *a, unsigned int index);
static bool check_list_index(List *a, unsigned int index);

static list_node* forward(list_node *initial_node, unsigned int begin, unsigned int end);
static list_node* backward(list_node *initial_node, unsigned int begin, unsigned int end);
static list_node* choose_shortest_path(List *a, unsigned int index, unsigned int x, unsigned int y, unsigned int z);

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
	new_list->last_index = 0;
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

static void list_insert_between(List *a, void *element, unsigned int index)
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

void list_insert(List *a, void *element, unsigned int index)
{
	if(index == a->size)
	{
		list_append(a, element);
	}
	else if(check_list_index(a, index))
	{
		list_insert_between(a, element, index);
	}
	else
	{
		perror("Invalid index in list\n");
	}
	return;
}

void print_list
(
	List *a,
	void (*print_element)(void *element, void *params),
	void *parameters
)
{
	list_node *i;
	for(i = a->head->next; i->next; i = i->next)
	{
		print_element(i->data, parameters);
	}
	return;
}

#ifdef DEBUG
void debug_print_list(List *a)
{
	list_node *i;
	printf("List\nHead %p\nTail %p\nLast used %p\nLast index %d\n\n", a->head, a->tail, a->last_used, a->last_index);
	for(i = a->head; i; i = i->next)
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
#endif

void* list_at_pos(List *a, unsigned int index)
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

bool list_delete
(
	List *a,
	unsigned int index,
	void (*free_element)(void *element, void *params),
	void *parameters
)
{
	list_node *deleted;
	if(!check_list_index(a, index))
		return false;

	deleted = get_node_at_index(a, index);
	deleted->previous->next = deleted->next;
	deleted->next->previous = deleted->previous;
	a->size--;
	free_element(deleted->data, parameters);
	free(deleted);

	if(a->last_index == index)
	{
		a->last_used = NULL;
		a->last_index = 0;
	}
	return true;
}

void delete_list
(
	List *a,
	void (*free_element)(void *element, void *params),
	void *parameters
)
{
	list_node *tmp;
	for(tmp = a->head->next; tmp->next; tmp = tmp->next)
	{
		if(tmp->data)
		{
			free_element(tmp->data, parameters);
		}
		free(tmp->previous);
	}
	/*free last elements*/
	free(tmp->previous);
	free(tmp);
	free(a);
	return;
}

static list_node* get_node_at_index(List *a, unsigned int index)
{
	list_node *ret;
	unsigned int x, y, z;
	/*between 0 and index*/
	x = index;
	/*between current index and last used index*/
	if(index > a->last_index)
		y = index - a->last_index;
	else
		y = a->last_index - index;
	/*y = abs(index - a->last_index);*/
	/*between last element and index*/
	z = a->size - index - 1;
	ret = choose_shortest_path(a, index, x, y, z);

	a->last_used = ret;
	a->last_index = index;
	return ret;
}

static list_node* choose_shortest_path
(
	List *a,
	unsigned int index,
	unsigned int x,
	unsigned int y,
	unsigned int z
)
{
	list_node *ret;

	if(x < y && x < z)
	{
		/* begining of the list */
		ret = forward(a->head->next, 0, index);
	}
	else if(y < z && a->last_used)
	{
		/*near the last index */
		if(index > a->last_index)
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
		/* at the end of the list*/
		ret = backward(a->tail->previous, a->size-1, index);
	}
	return ret;
}

static list_node* forward
(
	list_node *initial_node,
	unsigned int begin,
	unsigned int end
)
{
	unsigned int i;
	for(i = begin; i < end; i++)
	{
		initial_node = initial_node->next;
	}
	return initial_node;
}

static list_node* backward
(
	list_node *initial_node,
	unsigned int begin,
	unsigned int end
)
{
	unsigned int i;
	for(i = begin; i > end; i--)
	{
		initial_node = initial_node->previous;
	}
	return initial_node;
}

static bool check_list_index(List *a, unsigned int index)
{
	if(index >= 0 && index < a->size)
	{
		return true;
	}
	return false;
}
