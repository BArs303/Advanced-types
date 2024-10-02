#include "advanced_types.h"

#ifndef HMAP_H
#define HMAP_H

typedef struct hash_map hmap;
struct hash_map
{
	char key;
	int hash;
	int value;
};
int function_hmap(const char *text);

#endif
