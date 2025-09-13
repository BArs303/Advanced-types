#ifndef _ADVANCED_TYPES_H
#define _ADVANCED_TYPES_H

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

#ifndef CAST
 #define CAST(type, value) ((type) value)
#endif

void ptr_swap(void *arg1, void *arg2);
void passive_destruct(void *element, void *params);

bool is_whitespace(char c, const char *whitespaces);
char* file_to_str(const char *filename);
char* int_to_str(unsigned int num);

/* USE  str_split INSTEAD */
char** mysplit(char *str, char separator, unsigned int *return_size);

void print_buffer(void *buffer, size_t size);
bool is_prime(int number);

char* str_duplicate(char *restrict str);
char** str_split(char *str, char *delims, unsigned int *n);

#endif
