#include "advanced_types.h"

#ifndef HMAP_H
#define HMAP_H
#define TABLE_SIZE 13

typedef struct dynamic_array hashtable;

struct noda
{
	char *key;
	int hash;
	int value;
};

unsigned int function_hmap(const char *key);
hashtable* init_hmap();
void hmap_insert(hashtable *a,struct noda *b);

#endif
