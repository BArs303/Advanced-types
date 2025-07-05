#ifndef JSON_H
#define JSON_H

#include "parsers.h"

union json_val
{
	int number;
	float fnumber;
	char *string;
	List *list;
	HMap *object;
	bool jsbool;
};

typedef union json_val JSON_value;

struct json
{
	JSON_value value;
	Types type;
};
typedef struct json JSON;
extern const char default_whitespaces[];

JSON* json_parse(char *str);
JSON* json_parse_file(const char *filename);
char* json_stringify(JSON *a);
void delete_json(void *element, void *params);
#endif
