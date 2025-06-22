#include "hmap.h"

static unsigned int hash_function(const char *key);
static HNode* init_hash_node(char *key, void *value);
static void print_hash_values(void *element, void *params);
static void expand_hash_map(HMap *a);

static void print_hash_node(void *element, void *params);

static void delete_bucket(void *element, void *parameters);
static void delete_hnode(void *element, void *parameters);

HMap* init_hmap()
{
	HMap *a;
	a = malloc(sizeof(HMap));
	a->array = init_darray_with_length(HMAP_DEFAULT_SIZE);
	return a;
}

void delete_hmap
(
	HMap *a,
	void (*free_element)(void *element, void *params),
	void *parameters
)
{
	struct nested_del user_params;
	user_params.user_function = free_element;
	user_params.parameters = parameters;
	delete_darray(a->array, &delete_bucket, &user_params);
}

static void delete_bucket(void *element, void *parameters)
{
	delete_list(element, &delete_hnode, parameters);
}

static void delete_hnode(void *element, void *parameters)
{
	struct nested_del *user_params;
	user_params = parameters;
	user_params->user_function(CAST(HNode*, element), user_params->parameters);
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

static HNode* init_hash_node(char *key, void *value)
{
	HNode *node;
	node = malloc(sizeof(HNode));
	node->key = key;
	node->value = value;
	return node;
}
void hmap_insert(HMap *a, char *key, void *value)
{
	HNode *node;
	List *values;
	unsigned int index;
	node = init_hash_node(key, value);
	index = hash_function(key);
	//printf("before hmap insert. index %u, capacity %d\n", index, a->array->capacity);
	index = index % a->array->size;
	values = darray_at_pos(a->array, index);
	if(!values)
	{
		values = init_list();
	}
	list_append(values, node);
	darray_replace(a->array, values, index, &passive_destruct, NULL);

	if(values->size > a->array->size/ 10 * 2)
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
	new_array = init_darray_with_length(old_array->size* step);

	for(int i = 0; i < old_array->size; i++)
	{
		old_values = darray_at_pos(old_array, i);
		if(!old_values)
			continue;
		for(int j = 0; j < old_values->size; j++)
		{
			node = list_at_pos(old_values, j);

			index = hash_function(node->key) % new_array->size;
			new_values = darray_at_pos(new_array, index);
			if(!new_values)
			{
				new_values = init_list();
			}

			list_append(new_values, node);
			darray_replace(new_array, new_values, index, &passive_destruct, NULL);
		}
		//all nodes are copied to the new array,
		//so delete function is not required
		delete_list(old_values, &passive_destruct, NULL);
	}
	//all values in the array have already been cleared
	delete_darray(old_array, &passive_destruct, NULL);
	a->array = new_array;
	return;
}

void* hmap_get(HMap *a, const char *key)
{
	HNode *n;
	List *values;
	unsigned int index;

	index = hash_function(key) % a->array->size;
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
	print_darray(a->array, print_hash_values, NULL);
}

static void print_hash_values(void *element, void *params)
{
	if(element)
	{
		print_list(element, &print_hash_node, params);
	}
	printf("Empty index\n");
}
static void print_hash_node(void *element, void *params)
{
	HNode *node;
	int *a;
	node = element;
	a = node->value;
	printf("key %s value %d\n", node->key, *a);
}

List* hmap_to_list(HMap *a)
{
	unsigned int i, j;
	Darray *values;
	HNode *n;
	List *result, *bucket;
	if(a)
	{
		result = init_list();
		values = a->array;
		for(i = 0; i < values->size; i++)
		{
			bucket = darray_at_pos(values, i);
			if(bucket)
			{
				for(j = 0; j < bucket->size; j++)
				{
					n = list_at_pos(bucket, j);
					list_append(result, n);
				}
			}
		}
	}
	else
	{
		result = NULL;
	}
	return result;
}
