#include "advanced_types.h"

#ifndef SET_H
#define SET_H

typedef struct dynamic_array Set;

Set* init_set();
Set* init_set_with_length(int length);
void destruct_set(Set *a, void(*free_element)(void *element));
void print_set(Set *a, void (*print_element)(void *element));

void set_clear(Set *a, void (*free_element)(void *element));
bool set_remove(Set *a, int index, void(*free_element)(void *element));

bool set_insert(Set *a, void *element);
bool s_set_insert(Set *a, void *element, int (*sort_criteria)(void *arg1, void *arg2));

void* set_binary_search(Set *a, void *element, int (*sort_criteria)(void *arg1, void *arg2));
int set_search_index(Set *a, void *element, int (*sort_criteria)(void *arg1, void *arg2));
bool check_existence(Set *a, void *element, int (*sort_criteria)(void *arg1, void *arg2));

void* set_at_pos(Darray *a, int index);
#endif
