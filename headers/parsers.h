#ifndef PARSERS_H
#define PARSERS_H

#include "dlinked_list.h"
#include "hmap.h"

struct parsable_data
{
	char *str;
	int error;
	unsigned int level;
};

enum te
{
	type_object,
	type_list,
	type_bool,
	type_darray,
	type_number,
	type_string
};

typedef enum te Types;
typedef struct parsable_data PData;

extern const char default_whitespaces[];

char* cut_from_buffer(PData *buffer, size_t length);
size_t skip_whitespaces(PData *buffer, const char *whitespaces);
char* my_concat(char *dst, const char *src);

#endif
