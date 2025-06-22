#ifndef HASH_MAP_H
#define HASH_MAP_H
#define HMAP_DEFAULT_SIZE 7

#include "dynamic_array.h"
#include "dlinked_list.h"

typedef struct hash_map HMap;
typedef struct hmap_node HNode;

struct hash_map
{
	Darray *array;
};

struct hmap_node
{
	char *key;
	void *value;
};

struct nested_del
{
	void *parameters;
	void (*user_function)(void *element, void *params);
};

HMap* init_hmap();
void delete_hmap
(
	HMap *a,
	void (*free_element)(void *element, void *params),
	void *params
);
void hmap_insert(HMap *a, char *key, void *element);
void* hmap_get(HMap *a, const char *key);
List* hmap_to_list(HMap *a);
void print_hmap(HMap *a);
#endif
