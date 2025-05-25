#include "advanced_types.h"

#ifndef MYSTRING_H
#define MYSTRING_H

char* file_to_str(const char *filename);
int str_to_int(char *str);
char* int_to_str(int num);
char* concat(char *dest, const char *src);

bool is_whitespace(char c, const char *whitespaces);
size_t read_until(const char *str, const char *limiters);

char** mysplit(const char *str, const char separators, unsigned int *return_size);
#endif
