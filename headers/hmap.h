#include "advanced_types.h"

#ifndef HASH_MAP_H
#define HASH_MAP_H
#define HMAP_DEFAULT_SIZE 7

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


HMap* init_hmap();
void hmap_insert(HMap *a, const char *key, void *element);
void* hmap_get(HMap *a, const char *key);
void print_hmap(HMap *a);
#endif
