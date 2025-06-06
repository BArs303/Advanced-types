#ifndef JSON_H
#define JSON_H

#include "parsers.h"

union json_val
{
	int number;
	float fnumber;
	char *string;
	List *list;
	List *object; /*need to replace with normal data type*/
	bool jbool;
};
typedef union json_val JSON_value;

struct json
{
	char *key;
	JSON_value value;
	Types type;
};
typedef struct json JSON;

JSON* json_parse(char *str);
JSON* json_parse_file(const char *filename);
char* json_stringify(JSON *a);
void delete_json(void *a);
#endif
