#include "json.h"

static char* get_key(char **buffer);
static JSON* get_value(char **buffer);

static int read_until(char *str, const char *limiters);
static char* copy_from_buffer(char **buffer, int length);
static void skip_whitespaces(char **buffer, bool (*is_whitespace)(char c));
static bool default_whitespaces(char c);

static JSON* coordinator(char **buffer);

static JSON* parse_string(char **buffer);
static JSON* parse_object(char **buffer);
static JSON* parse_list(char **buffer);
static JSON* parse_int(char **buffer);

static char* stringify_object(JSON *a);
static char* stringify_list(JSON *a);
static char* stringify_string(JSON *a);
static char* stringify_int(JSON *a);
static char* process_key(JSON *a);

JSON* json_parse_file(const char *fn)
{
	char *str;
	JSON *a;
	str = file_to_str(fn);
	a = json_parse(str);
	free(str);
	return a;
}

JSON* json_parse(char *str)
{
	JSON *result = NULL;
	char **buffer = &str;

	skip_whitespaces(buffer, &default_whitespaces);

	if(**buffer == '{')
	{
		result = parse_object(buffer);
		result->key = NULL;
	}
	return result;
}

static JSON* parse_object(char **buffer)
{
	List *object_fields;
	JSON *result, *field;
	char *key;

	object_fields = init_list();
	result = malloc(sizeof(JSON));
	(*buffer)++; //skip { symb
	//printf("starting parse object\n");
	while(**buffer) 
	{
		if(**buffer == '}')
		{
			break;
		}
		else if(**buffer == '"')
		{
			key = get_key(buffer);
			//printf("key: %s\n", key);
			field = get_value(buffer);
			//need check field != 0
			field->key = key;
			list_append(object_fields, field);
		}
		else if(**buffer == ',')
		{
			(*buffer)++;
		}
		skip_whitespaces(buffer, &default_whitespaces);
	}

	//printf("object ended %c %d\n", **buffer, **buffer);
	(*buffer)++; //skip } symb 
	result->value.object = object_fields;
	result->type = type_Object;
	return result;
}

static JSON* parse_list(char **buffer)
{
	JSON *result, *element;
	List *array;

	result = malloc(sizeof(JSON));
	array = init_list();
	(*buffer)++; //skip [ symb
	//printf("starting parse list\n");
	do
	{
		if(**buffer == ',')
			(*buffer)++;
		skip_whitespaces(buffer, &default_whitespaces);
		element = coordinator(buffer);
		element->key = NULL;
		list_append(array, element);
	}while(**buffer == ',');

	skip_whitespaces(buffer, &default_whitespaces);

	if(**buffer != ']')
	{
		printf("error: not closed array\n");
	}
	else
	{
		(*buffer)++; //skip ] symb
		skip_whitespaces(buffer, default_whitespaces);
	}

	//printf("array ended %c %d\n", **buffer, **buffer);
	result->value.list = array;
	result->type = type_List;
	return result;
}

static JSON* parse_string(char **buffer)
{
	JSON *a;
	int length;
	const char *limiter = "\"";
	a = malloc(sizeof(JSON));

	(*buffer)++;//skip first " symb
	length = read_until(*buffer, limiter);
	a->value.string = copy_from_buffer(buffer, length);
	(*buffer)++;//skip last " symb
	a->type = type_String;
	return a;
}

static JSON* parse_int(char **buffer)
{
	JSON *a;
	char *number;
	int length;
	const char *limiters = ",\n";

	a = malloc(sizeof(JSON));
	length = read_until(*buffer, limiters);
	number = copy_from_buffer(buffer, length);

	a->value.number = str_to_int(number);
	a->type = type_Int;
	free(number);
	return a;
}

static char* get_key(char **buffer)
{
	char *key;
	int length;
	const char *limiter = ":";

	length = read_until(*buffer, limiter);
	(*buffer)++; //skip first " symb
	length -= 2; //don't copy " symbols
	key = copy_from_buffer(buffer, length);
	(*buffer)++;//skip last " symb
	return key;
}

static int read_until(char *str, const char *limiters)
{
	for(int i = 0; str[i]; i++)
	{
		for(int j = 0; limiters[j]; j++)
		{
			if(str[i] == limiters[j])
				return i;
		}
	}
	return -1;
}

static char* copy_from_buffer(char **buffer, int length)
{
	char *ret;
	ret = malloc(sizeof(char) * (length+1));
	for(int i = 0; i < length; i++, (*buffer)++)
	{
		ret[i] = **buffer;
	}
	ret[length] = 0;
	return ret;
}

static JSON* get_value(char **buffer)
{
	JSON *value;
	(*buffer)++;//skip : symb
	skip_whitespaces(buffer, &default_whitespaces);
	value = coordinator(buffer);
	return value;
}

static JSON* coordinator(char **buffer)
{
	JSON *value;
	if(**buffer == '[')
	{
		value = parse_list(buffer);
	}
	else if(**buffer == '{')
	{
		value = parse_object(buffer);
	}
	else if(**buffer == '"')
	{
		value = parse_string(buffer);
	}
	else if(**buffer >= '0' && **buffer <= '9')
	{
		value = parse_int(buffer);
	}
	else
	{
		value = NULL;
		printf("Unknown type\n");
	}
	return value;
}

static void skip_whitespaces(char **buffer, bool (*is_whitespace)(char c))
{
	while(**buffer && is_whitespace(**buffer))
	{
		(*buffer)++;
	}
}

static bool default_whitespaces(char c)
{
	char wspaces[] = {'\n', ' ', '\t'};
	int n = 3;
	for(int i = 0; i < n; i++)
	{
		if(wspaces[i] == c)
			return true;
	}
	return false;
}


/*Unload JSON data to file*/

char* json_stringify(JSON *a)
{
	char *ret = NULL;
	switch(a->type)
	{
		case type_String:
		ret = stringify_string(a);
		//printf("String value %s\n", ret);
		break;

		case type_Int:
		ret = stringify_int(a);
		//printf("Int value %s\n", ret);
		break;

		case type_Object:
		ret = stringify_object(a);
		//printf("Object value %s\n", ret);
		break;

		case type_List:
		ret = stringify_list(a);
		//printf("List value %s\n", ret);
		break;

		default:;
	}
	return ret;
}

static char* stringify_object(JSON *a)
{
	char *result;
	List *tmp = a->value.object;
	result = process_key(a);
	result = concat(result, "\n{\n");
	for(int i = 0; i < tmp->size-1; i++)
	{
		result = concat(result, "    ");
		result = concat(result, json_stringify((JSON*)list_at_pos(tmp, i)));
		result = concat(result, ",\n");
	}
	result = concat(result, "    ");
	result = concat(result, json_stringify((JSON*)list_at_pos(tmp, tmp->size-1)));
	result = concat(result, "\n}");
	return result;
}
static char* stringify_list(JSON *a)
{
	char *result;
	List *tmp = a->value.list;
	result = process_key(a);
	result = concat(result, "\n[\n");
	for(int i = 0; i < tmp->size-1; i++)
	{
		result = concat(result, "    ");
		result = concat(result, json_stringify((JSON*)list_at_pos(tmp, i)));
		result = concat(result, ",\n");
	}
	result = concat(result, "    ");
	result = concat(result, json_stringify((JSON*)list_at_pos(tmp, tmp->size-1)));
	result = concat(result, "\n]");
	return result;
}

static char* stringify_int(JSON *a)
{
	char *result;
	result = process_key(a);
	result = concat(result, " ");
	result = concat(result, int_to_str(a->value.number));
	return result;
}

static char* stringify_string(JSON *a)
{
	char *result;
	result = process_key(a);
	result = concat(result, " \"");
	result = concat(result, a->value.string);
	result = concat(result,"\"");
	return result;
}

static char* process_key(JSON *a)
{
	char *result;
	result = malloc(sizeof(char) * 3);//3 = ""\0 Start of the string
	if(a->key)
	{
		result[0] = '"';
		result[1] = 0;
		result = concat(result, a->key);
		result = concat(result, "\":");
	}
	else
	{
		result[0] = 0;
	}
	return result;
}
