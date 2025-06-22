#include "json.h"

static char* get_key(PData *buffer);
static JSON* get_value(PData *buffer);

static JSON* coordinator(PData *buffer);

static JSON* parse_string(PData *buffer);
static JSON* parse_object(PData *buffer);
static JSON* parse_list(PData *buffer);
static JSON* parse_int(PData *buffer);
static JSON* parse_bool(PData *buffer);

static char* stringify_object(JSON *a, int level);
static char* stringify_list(JSON *a, int level);
static char* stringify_string(JSON *a, int level);
static char* stringify_int(JSON *a, int level);
static char* stringify_bool(JSON *a, int level);

static char* process_key(HNode *a, int level);
static char* l_json_stringify(JSON *a, int level);
static char* add_tabs(int level);

static void default_json_free(JSON_value data, Types type, void *params);

JSON* json_parse_file(const char *filename)
{
	char *str;
	JSON *a;
	str = file_to_str(filename);
	a = json_parse(str);
	free(str);
	return a;
}

JSON* json_parse(char *str)
{
	PData buffer;
	JSON *result;

	result = NULL;
	buffer.str = str;

	skip_whitespaces(&buffer, default_whitespaces);

	if(*(buffer.str) == '{')
	{
		result = parse_object(&buffer);
		result->key = NULL;
	}
	return result;
}

static JSON* parse_object(PData *buffer)
{
	HMap *object;
	JSON *result, *field;
	char *key;

	object = init_hmap();
	result = malloc(sizeof(JSON));
	(*(buffer->str))++; /*skip { symb*/
	while(*(buffer->str)) 
	{
		if(*(buffer->str) == '}')
		{
			break;
		}
		else if(*(buffer->str) == '"')
		{
			key = get_key(buffer);
			field = get_value(buffer);
			/*need check field != 0*/
			field->key = key;
			hmap_insert(object, key, field);
		}
		else if(*(buffer->str) == ',')
		{
			buffer->str++;
		}
		skip_whitespaces(buffer, default_whitespaces);
	}

	buffer->str++; /*skip } symb */
	result->value.object = object;
	result->type = type_object;
	return result;
}

static JSON* parse_list(PData *buffer)
{
	JSON *result, *element;
	List *array;

	result = malloc(sizeof(JSON));
	array = init_list();
	buffer->str++; /*skip [ symb*/
	do
	{
		if(*(buffer->str) == ',')
			buffer->str++;
		skip_whitespaces(buffer, default_whitespaces);
		element = coordinator(buffer);
		element->key = NULL;
		list_append(array, element);
	}while(*(buffer->str) == ',');

	skip_whitespaces(buffer, default_whitespaces);

	if(*(buffer->str) != ']')
	{
		perror("error: not closed array\n");
	}
	else
	{
		buffer->str++; /*skip ] symb*/
		skip_whitespaces(buffer, default_whitespaces);
	}

	result->value.list = array;
	result->type = type_list;
	return result;
}

static JSON* parse_string(PData *buffer)
{
	JSON *a;
	unsigned int length;
	a = malloc(sizeof(JSON));

	buffer->str++;/*skip first " symb*/
	for(length = 0; buffer->str[length] != '"'; length++)
	{
		if(buffer->str[length] == '\\')
			length++;
	}
	a->value.string = cut_from_buffer(buffer, length);
	buffer->str++; /*skip last " symb*/
	a->type = type_string;
	return a;
}

static JSON* parse_int(PData *buffer)
{
	JSON *a;
	char *number;
	size_t length;
	const char *limiters = ",\n";

	a = malloc(sizeof(JSON));
	length = strcspn(buffer->str, limiters);
	number = cut_from_buffer(buffer, length);

	a->value.number = atoi(number);
	a->type = type_number;
	free(number);
	return a;
}
static JSON* parse_bool(PData *buffer)
{
	JSON *a;
	char *var;
	size_t length;
	const char *limiters = ",\n";
	length = strcspn(buffer->str, limiters);
	var = cut_from_buffer(buffer, length);
	a = malloc(sizeof(JSON));
	a->type = type_bool;
	if(strcmp(var, "true") == 0)
	{
		a->value.jsbool = false;
	}
	else if(strcmp(var, "false") == 0)
	{
		a->value.jsbool = true;
	}
	else
	{
		free(a);
		return NULL;
	}
	return a;
}

static char* get_key(PData *buffer)
{
	char *key;
	size_t length;
	const char *limiter = ":";

	length = strcspn(buffer->str, limiter);
	buffer->str++; /*skip first " symb*/
	length -= 2; /*don't copy " symbols*/

	key = cut_from_buffer(buffer, length);
	buffer->str++;/*skip last " symb*/
	return key;
}

static JSON* get_value(PData *buffer)
{
	JSON *value;
	buffer->str++;/*skip : symb*/
	skip_whitespaces(buffer, default_whitespaces);
	value = coordinator(buffer);
	return value;
}

static JSON* coordinator(PData *buffer)
{
	JSON *value;
	if(*(buffer->str) == '[')
	{
		value = parse_list(buffer);
	}
	else if(*(buffer->str) == '{')
	{
		value = parse_object(buffer);
	}
	else if(*(buffer->str) == '"')
	{
		value = parse_string(buffer);
	}
	else if(*(buffer->str) >= '0' && *(buffer->str) <= '9')
	{
		value = parse_int(buffer);
	}
	else
	{
		value = parse_bool(buffer);
		if(value)
			return value;
		perror("Unknown type\n");
	}
	return value;
}

/*Unload JSON data to file*/
char* json_stringify(JSON *a)
{
	return my_concat(l_json_stringify(a, 0), "\n");
}

static char* l_json_stringify(JSON *a, int level)
{
	char *ret = NULL;
	if(a)
	{
		switch(a->type)
		{
			case type_string:
			ret = stringify_string(a, level);
			/*printf("String value %s\n", ret);*/
			break;

			case type_number:
			ret = stringify_int(a, level);
			/*printf("Int value %s\n", ret);*/
			break;

			case type_object:
			ret = stringify_object(a, level);
			/*printf("Object value %s\n", ret);*/
			break;

			case type_list:
			ret = stringify_list(a, level);
			/*printf("List value %s\n", ret);*/
			break;

			case type_bool:
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
	List *tmp;
	HNode *list_element;
	tmp = hmap_to_list(a->value.object);
	result = malloc(sizeof(char));
	result = my_concat(result, "\n");
	result = my_concat(result, add_tabs(level));
	result = my_concat(result, "{\n");
	for(int i = 0; i < tmp->size-1; i++)
	{
		result = my_concat(result, add_tabs(level+1));

		list_element = list_at_pos(tmp, i);
		result = my_concat(result, process_key(list_element, level));
		
		result = my_concat(result, l_json_stringify((JSON*)list_element->value, level+1));
		result = my_concat(result, ",\n");
	}
	result = my_concat(result, add_tabs(level+1));
	list_element = list_at_pos(tmp, tmp->size-1);
	result = my_concat(result, l_json_stringify(list_element->value, level+1));
	result = my_concat(result, "\n");
	result = my_concat(result, add_tabs(level));
	result = my_concat(result, "}");

	return result;
}
static char* stringify_list(JSON *a, int level)
{
	char *result;
	List *tmp = a->value.list;
	unsigned int i;
	result = malloc(sizeof(char));
	result = my_concat(result, "\n");
	result = my_concat(result, add_tabs(level));
	result = my_concat(result, "[\n");
	for(i = 0; i < tmp->size-1; i++)
	{
		result = my_concat(result, add_tabs(level+1));
		result = my_concat(result, l_json_stringify((JSON*)list_at_pos(tmp, i), level+1));
		result = my_concat(result, ",\n");
	}
	result = my_concat(result, add_tabs(level+1));
	result = my_concat(result, l_json_stringify((JSON*)list_at_pos(tmp, tmp->size-1), level+1));
	result = my_concat(result, "\n");
	result = my_concat(result, add_tabs(level));
	result = my_concat(result, "]");
	return result;
}

static char* stringify_int(JSON *a, int level)
{
	char *result;
	result = malloc(sizeof(char));
	result = my_concat(result, int_to_str(a->value.number));
	return result;
}
static char* stringify_bool(JSON *a, int level)
{
	char *result;
	result = malloc(sizeof(char));
	if(a->value.jsbool)
		result = my_concat(result, "true");
	else
		result = my_concat(result, "false");
	return result;
}

static char* stringify_string(JSON *a, int level)
{
	char *result, *tmp;
	unsigned int i, j, length;

	result = malloc(sizeof(char));
	result = my_concat(result, " \"");

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
		result = my_concat(result, tmp);
	}
	else
	{
		result = my_concat(result, a->value.string);
	}
	result = my_concat(result,"\"");
	return result;
}

static char* process_key(HNode *a, int level)
{
	char *result;
	result = add_tabs(level);
	if(a->key)
	{
		result[0] = '"';
		result[1] = 0;
		result = my_concat(result, a->key);
		result = my_concat(result, "\":");
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
		str = my_concat(str, tab);
	}
	return str;
}
/*Delete section*/
void delete_json(void *element, void *params)
{
	JSON *a;
	a = element;
	if(a)
	{
		default_json_free(a->value, a->type, params);
		free(a->key);
		free(a);
	}
}

static void default_json_free(JSON_value data, Types type, void *params)
{
	switch(type)
	{
	case type_string:
		free(data.string);
	break;

	case type_list:
		delete_list(data.list, &delete_json, params);
	break;

	case type_object:
		delete_hmap(data.object, &delete_json, params);
	break;

	case type_number:
	break;

	case type_bool:
	break;

	default:
		perror("Error: Unknown JSON type\n");
	}
}
