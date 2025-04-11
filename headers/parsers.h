#ifndef PARSERS_H
#define PARSERS_H

#include "advanced_types.h"
#include "json.h"
#include "yaml_parser.h"


struct parsable_data
{
	char *str;
	int error;
	unsigned int lengthcpy;
	unsigned int level;
};

typedef struct parsable_data PData;

char* ycut_from_buffer(PData *buffer);
unsigned int yskip_whitespaces(PData *buffer, const char *whitespaces);
unsigned int yread_until(char *str, const char *limiters);

#endif
