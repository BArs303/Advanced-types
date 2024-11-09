#include "advanced_types.h"

#ifndef HASH_MAP_H
#define HASH_MAP_H

typedef struct hmap Hmap;
typedef struct hmap_node Hnode;

struct hmap_node
{
	char *key;
	void *value;
};

struct hmap
{
	void **array;
	unsigned int size;
};

struct hmap_object
{
	void *data;
	bool single_value;
};

Hmap* init_hmap();
void hmap_insert(Hmap *a, void *element, char *key);
void* hmap_get(Hmap *a, char *key);
#endif
