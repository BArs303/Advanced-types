#include "hmap.h"

unsigned int function_hmap(const char *key){
	unsigned int hash = 0;
	while (*key) {
        hash = (hash << 2) + *key++;
    }
	return hash % TABLE_SIZE;
}
hashtable* init_hmap(){
	hashtable *a = malloc(sizeof(hashtable));
	a->array = init_darray_with_length(TABLE_SIZE);
	darray_append(a,NULL);
	return a;
}
void hmap_insert(hashtable *a,struct noda *b){
	int i = function_hmap(b->key);
	darray_insert(a,b,i);
}
