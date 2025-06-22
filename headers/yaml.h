#ifndef YAML_PARSER_H
#define YAML_PARSER_H

#include "parsers.h"

typedef struct yaml YAML;

union yaml_value
{
	List *sequence;
	char *literal;
	HMap *object;
};

struct yaml
{
	union yaml_value value;
	Types type;
};
extern const char default_whitespaces[];

YAML* yaml_parse_file(const char *filename);
YAML* yaml_parse(char *str);
#endif
