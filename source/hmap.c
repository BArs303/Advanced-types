#include "hmap.h"

static unsigned int hash_function(const char *key);
static HNode* init_hash_node(const char *key, void *value);
static void print_hash_values(void *element);
static void expand_hash_map(HMap *a);

static void print_hash_node(void *element);


HMap* init_hmap()
{
	HMap *a;
	a = malloc(sizeof(HMap));
	a->array = init_darray_with_length(HMAP_DEFAULT_SIZE);
	return a;
}

static unsigned int hash_function(const char *key)
{
	unsigned int hash = 0;
	for(; *key; key++)
	{
		hash = *key + (hash << 6) + (hash << 16) - hash;
	}
	return hash;
}

static HNode* init_hash_node(const char *key, void *value)
{
	HNode *node;
	node = malloc(sizeof(HNode));
	node->key = key;
	node->value = value;
	return node;
}
void hmap_insert(HMap *a, const char *key, void *value)
{
	HNode *node;
	List *values;
	unsigned int index;
	node = init_hash_node(key, value);
	index = hash_function(key);
	printf("before hmap insert. index %u, capacity %d\n", index, a->array->capacity);
	index = index % a->array->capacity;
	values = darray_at_pos(a->array, index);
	if(!values)
	{
		values = init_list();
	}
	list_append(values, node);
	darray_replace(a->array, values, index, &passive_destruct);

	if(values->size > a->array->capacity / 2)
	{
		expand_hash_map(a);
	}
	return;
}

static void expand_hash_map(HMap *a)
{
	Darray *old_array, *new_array;
	HNode *node;
	List *old_values, *new_values;
	unsigned int index;
	int step = 2;

	old_array = a->array;
	new_array = init_darray_with_length(old_array->capacity * step);

	for(int i = 0; i < old_array->capacity; i++)
	{
		old_values = darray_at_pos(old_array, i);
		if(!old_values)
			continue;
		for(int j = 0; j < old_values->size; j++)
		{
			node = list_at_pos(old_values, j);

			index = hash_function(node->key) % new_array->capacity;
			new_values = darray_at_pos(new_array, index);
			if(!new_values)
			{
				new_values = init_list();
			}

			list_append(new_values, node);
			darray_replace(new_array, new_values, index, &passive_destruct);
		}
		//all nodes are copied to the new array,
		//so delete function is not required
		delete_list(old_values, &passive_destruct);
	}
	//all values in the array have already been cleared
	delete_darray(old_array, &passive_destruct);
	a->array = new_array;
	return;
}

void* hmap_get(HMap *a, const char *key)
{
	HNode *n;
	List *values;
	unsigned int index;

	index = hash_function(key) % a->array->capacity;
	values = darray_at_pos(a->array, index);
	if(values)
	{
		for(int i = 0; i < values->size; i++)
		{
			n = list_at_pos(values, i);
			if(strcmp(key, n->key) == 0)
				return n->value;
		}
	}
	return NULL;
}

void print_hmap(HMap *a)
{
	print_darray(a->array, print_hash_values);
}

static void print_hash_values(void *element)
{
	if(element)
	{
		print_list(element, print_hash_node);
	}
	printf("Empty index\n");
}
static void print_hash_node(void *element)
{
	HNode *node;
	int *a;
	node = element;
	a = node->value;
	printf("key %s value %d\n", node->key, *a);
}

