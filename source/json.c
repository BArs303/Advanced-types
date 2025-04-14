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
static JSON* parse_bool(char **buffer);

static char* stringify_object(JSON *a, int level);
static char* stringify_list(JSON *a, int level);
static char* stringify_string(JSON *a, int level);
static char* stringify_int(JSON *a, int level);
static char* stringify_bool(JSON *a, int level);

static char* process_key(JSON *a, int level);
static char* l_json_stringify(JSON *a, int level);
static char* add_tabs(int level);

static void default_json_free(JSON_value data, Types type);

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
	(*buffer)++; /*skip { symb*/
	while(**buffer) 
	{
		if(**buffer == '}')
		{
			break;
		}
		else if(**buffer == '"')
		{
			key = get_key(buffer);
			field = get_value(buffer);
			/*need check field != 0*/
			field->key = key;
			list_append(object_fields, field);
		}
		else if(**buffer == ',')
		{
			(*buffer)++;
		}
		skip_whitespaces(buffer, &default_whitespaces);
	}

	(*buffer)++; /*skip } symb */
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
	(*buffer)++; /*skip [ symb*/
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
		fprintf(stderr, "error: not closed array\n");
	}
	else
	{
		(*buffer)++; /*skip ] symb*/
		skip_whitespaces(buffer, default_whitespaces);
	}

	result->value.list = array;
	result->type = type_List;
	return result;
}

static JSON* parse_string(char **buffer)
{
	JSON *a;
	unsigned int length;
	a = malloc(sizeof(JSON));

	(*buffer)++;/*skip first " symb*/
	for(length = 0; (*buffer)[length] != '"'; length++)
	{
		if((*buffer)[length] == '\\')
			length++;
	}
	a->value.string = copy_from_buffer(buffer, length);
	(*buffer)++;/*skip last " symb*/
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
static JSON* parse_bool(char **buffer)
{
	JSON *a;
	char *var;
	int length;
	const char *limiters = ",\n";
	length = read_until(*buffer, limiters);
	var = copy_from_buffer(buffer, length);
	a = malloc(sizeof(JSON));
	a->type = type_Bool;
	if(strcmp(var, "true") == 0)
	{
		a->value.jbool = false;
	}
	else if(strcmp(var, "false") == 0)
	{
		a->value.jbool = true;
	}
	else
	{
		free(a);
		return NULL;
	}
	return a;
}

static char* get_key(char **buffer)
{
	char *key;
	int length;
	const char *limiter = ":";

	length = read_until(*buffer, limiter);
	(*buffer)++; /*skip first " symb*/
	length -= 2; /*don't copy " symbols*/
	key = copy_from_buffer(buffer, length);
	(*buffer)++;/*skip last " symb*/
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
	(*buffer)++;/*skip : symb*/
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
		value = parse_bool(buffer);
		if(value)
			return value;
		fprintf(stderr, "Unknown type\n");
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
	return concat(l_json_stringify(a, 0), "\n");
}

static char* l_json_stringify(JSON *a, int level)
{
	char *ret = NULL;
	if(a)
	{
		switch(a->type)
		{
			case type_String:
			ret = stringify_string(a, level);
			/*printf("String value %s\n", ret);*/
			break;

			case type_Int:
			ret = stringify_int(a, level);
			/*printf("Int value %s\n", ret);*/
			break;

			case type_Object:
			ret = stringify_object(a, level);
			/*printf("Object value %s\n", ret);*/
			break;

			case type_List:
			ret = stringify_list(a, level);
			/*printf("List value %s\n", ret);*/
			break;

			case type_Bool:
			ret = stringify_bool(a, level);
			/*printf("Bool value %s\n", ret);*/
			break;
			default:;
		}
	}
	return ret;
}

static char* stringify_object(JSON *a, int level)
{
	char *result;
	List *tmp = a->value.object;
	result = process_key(a, level);
	result = concat(result, "\n");
	result = concat(result, add_tabs(level));
	result = concat(result, "{\n");
	for(int i = 0; i < tmp->size-1; i++)
	{
		result = concat(result, add_tabs(level+1));
		result = concat(result, l_json_stringify((JSON*)list_at_pos(tmp, i), level+1));
		result = concat(result, ",\n");
	}
	result = concat(result, add_tabs(level+1));
	result = concat(result, l_json_stringify((JSON*)list_at_pos(tmp, tmp->size-1), level+1));
	result = concat(result, "\n");
	result = concat(result, add_tabs(level));
	result = concat(result, "}");

	return result;
}
static char* stringify_list(JSON *a, int level)
{
	char *result;
	List *tmp = a->value.list;
	unsigned int i;
	result = process_key(a, level);
	result = concat(result, "\n");
	result = concat(result, add_tabs(level));
	result = concat(result, "[\n");
	for(i = 0; i < tmp->size-1; i++)
	{
		result = concat(result, add_tabs(level+1));
		result = concat(result, l_json_stringify((JSON*)list_at_pos(tmp, i), level+1));
		result = concat(result, ",\n");
	}
	result = concat(result, add_tabs(level+1));
	result = concat(result, l_json_stringify((JSON*)list_at_pos(tmp, tmp->size-1), level+1));
	result = concat(result, "\n");
	result = concat(result, add_tabs(level));
	result = concat(result, "]");
	return result;
}

static char* stringify_int(JSON *a, int level)
{
	char *result;
	result = process_key(a, level);
	result = concat(result, " ");
	result = concat(result, int_to_str(a->value.number));
	return result;
}
static char* stringify_bool(JSON *a, int level)
{
	char *result;
	result = process_key(a, level);
	result = concat(result, " ");
	if(a->value.jbool)
		result = concat(result, "true");
	else
		result = concat(result, "false");
	return result;
}

static char* stringify_string(JSON *a, int level)
{
	char *result, *tmp;
	unsigned int i, j, length;
	result = process_key(a, level);
	result = concat(result, " \"");

	for(i = 0, length = 0; a->value.string[i]; i++, length++)
	{
		if(a->value.string[i] == '"')
			length++;
	}
	if(i != length)
	{
		tmp = malloc(sizeof(char) * (length + 1));
		for(i = 0, j = 0; a->value.string[i]; i++, j++)
		{
			if(a->value.string[i] == '"')
			{
				tmp[j] = '\\';
				j++;
			}
			tmp[j] = a->value.string[i];
		}
		tmp[length] = 0;
		result = concat(result, tmp);
	}
	else
	{
		result = concat(result, a->value.string);
	}
	result = concat(result,"\"");
	return result;
}

static char* process_key(JSON *a, int level)
{
	char *result;
	result = add_tabs(level);
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

static char* add_tabs(int level)
{
	const char *tab = "    ";
	char *str; 
	str = malloc(sizeof(char) * 5); /*4 = 1 tab + Null terminator*/
	*str = 0;
	for(int i = 0; i < level; i++)
	{
		str = concat(str, tab);
	}
	return str;
}
/*Delete section*/
void delete_json(void *element)
{
	JSON *a;
	a = element;
	if(a)
	{
		default_json_free(a->value, a->type);
		free(a->key);
		free(a);
	}
}

static void default_json_free(JSON_value data, Types type)
{
	switch(type)
	{
	case type_String:
		free(data.string);
	break;

	case type_List:
		delete_list(data.list, &delete_json);
	break;

	case type_Object:
		delete_list(data.object, &delete_json);/*replace to hashtable*/
	break;

	case type_Int:
	break;

	case type_Bool:
	break;

	default:
		fprintf(stderr, "Error: Unknown JSON type\n");
	}
}
