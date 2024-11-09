#include "hmap.h"

static unsigned int hash_function(char *key);

Hmap* init_hmap()
{
	Hmap *a;
	int table_size = 7;
	a = malloc(sizeof(Hmap));
	a->array = malloc(sizeof(void*) * table_size);
	for(int i = 0; i < table_size; i++)
	{
		a->array[i] = NULL;
	}
	a->size = table_size;
	return a;
}

static unsigned int hash_function(char *key)
{
	unsigned int hash = 0;
	for(; *key; key++)
	{
		hash = *key + (hash << 6) + (hash << 16) - hash;
	}
	return hash;
}

void hmap_insert(Hmap *a, void *element, char *key)
{
	Hnode *n;
	struct hmap_object *obj;
	List *values;
	unsigned int index;

	n = malloc(sizeof(Hnode));
	n->key = key;
	n->value = element;

	index = hash_function(key) % a->size;
	//printf("insert hash %d\n", index);

	if(!(a->array[index]))
	{
		obj = malloc(sizeof(struct hmap_object));
		//unique index
		//printf("first entry of hash\n");
		obj->data = n;
		obj->single_value = true;
		a->array[index] = obj;
		return;
	}
	obj = a->array[index];
	if(obj->single_value)
	{
		//printf("only one value with hash\n");
		values = init_list();
		list_append(values, obj->data);
		obj->data = values;
		obj->single_value = false;
	}
	list_append(obj->data, n);
	printf("related %f\n", (float)a->size /2);
	return;
}

void* hmap_get(Hmap *a, char *key)
{
	Hnode *n;
	List *values;
	struct hmap_object *obj;
	unsigned int index;

	index = hash_function(key) % a->size;
	printf("get index %d\n", index);
	if(a->array[index])
	{
		printf("value with hash exists\n");
		obj = a->array[index];
		if(obj->single_value)
		{
			printf("its unique hash in hashtable\n");
			n = obj->data;
			printf("key %s\n", n->key);
			if(strcmp(n->key, key) == 0)
				return n->value;
		}
		else
		{
			values = obj->data;
			printf("in hashtable %d values with hash\n", values->size);
			for(int i = 0; i < values->size; i++)
			{
				n = list_at_pos(values, i);
				printf("searching in list %d key %s hashtable key %s\n", i, key, n->key);
			}

			for(int i = 0; i < values->size; i++)
			{
				n = list_at_pos(values, i);
				if(strcmp(n->key, key) == 0)
					return n->value;
			}

		}
	}
	printf("hashtable value is null\n");
	return NULL;
}
