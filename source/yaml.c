#include "yaml.h"

/*
	char * - read only
	char [] - read/write
*/

char* special_chars = ":{}[]&*#?|<>=!%@\\ ";

static char* get_key(PData *buffer);
static YAML* get_value(PData *buffer);

static YAML* coordinator(PData *buffer);
static YAML* parse_object(PData *buffer);
static YAML* parse_string(PData *buffer);
static YAML* parse_sequence(PData *buffer);

static unsigned int count_spaces(PData *buffer);
static unsigned int get_current_level(PData *buffer);

YAML* yaml_parse_file(const char *filename)
{
	char *str;
	YAML *a;
	str = file_to_str(filename);
	a = yaml_parse(str);
	free(str);
	return a;
}

YAML* yaml_parse(char *str)
{
	YAML *result = NULL;
	PData *buffer;
	buffer = malloc(sizeof(PData));
	buffer->str = str;
	buffer->error = 0;
	buffer->level = 0;

	result = parse_object(buffer);
	return result;
}

static char* get_key(PData *buffer)
{
	char *key;
	unsigned int i;

	key = NULL;

	for(i = 0; buffer->str[i] != ':'; i++)
	{
		if(buffer->str[i] == '\n')
		{
			buffer->error = 2;
			return NULL;
		}
	}

	key = cut_from_buffer(buffer, i);
	/* skip : symb */
	buffer->str++;

	return key;
}

static YAML* get_value(PData *buffer)
{
	YAML *ret;
	skip_whitespaces(buffer, " ");
	switch(*(buffer->str))
	{
		case '\n':
			ret = coordinator(buffer);
		break;
		default:
			ret = parse_string(buffer);
	}
	return ret;
}

static YAML* parse_string(PData *buffer)
{
	YAML *ret;
	unsigned int length, spaces, offset, i;
	char *result;
	char quote;

	ret = malloc(sizeof(YAML));

	if(*(buffer->str) == '\'' || *(buffer->str) == '"')
	{
		quote = *(buffer->str);
		/* skip quote */
		buffer->str++;
		for(length = 0; buffer->str[length]; length++)
		{
			/* shielding */
			if(buffer->str[length] == '\\')
			{
				length++;
				continue;
			}
			if(buffer->str[length] == quote)
			{
				if(buffer->str[length+1] == quote)
				{
					length++;
					continue;
				}
				break;
			}
		}
		/* skip end quote */
		buffer->str++;
		result = cut_from_buffer(buffer, length);
	}
	else
	{
		for(length = 0, offset = 0; ;offset++, length++)
		{
			if(buffer->str[offset] == '\n')
			{
				/* str[offset] = \n therefore +1*/
				for(spaces = 0; buffer->str[offset + spaces +1] == ' '; spaces++)
				{}
				if(spaces > buffer->level * 2)
				{
					/* skip \n char and add space for ' ' char in result string*/
					/*length++;*/
					offset += spaces;
					continue;
				}
				break;
			}
		}
		result = malloc(sizeof(char) * (length + 1));
		for(i = 0; i < length; i++, buffer->str++)
		{
			result[i] = *buffer->str;
			if(*(buffer->str) == '\n')
			{
				result[i] = ' ';
				while(*(buffer->str + 1) == ' ')
					buffer->str++;
			}
		}
		result[length] = 0;
	}
	/* skip \n */
	buffer->str++;
	ret->value.literal = result;
	ret->type = type_string;

	/*printf("string value: %s\n", ret->value.literal);*/
	return ret;
}

static unsigned int count_spaces(PData *buffer)
{
	int i;
	for(i = 0; buffer->str[i] == ' '; i++)
	{}
	return i;
}
static YAML* coordinator(PData *buffer)
{
	unsigned int counter, line_level;

	if(*buffer->str == '\n')
	{
		buffer->str++;

		counter = count_spaces(buffer);
		line_level = counter / 2;
		if(counter % 2)
		{
			buffer->error = 1;
			return NULL;
		}
		else if(buffer->str[counter] == '-' && line_level == buffer->level)
		{
			return parse_sequence(buffer);
		}
		else if(line_level == ++buffer->level)
		{
			return parse_object(buffer);
		}
	}
	buffer->error = 1;
	return NULL;

}
static unsigned int get_current_level(PData *buffer)
{
	unsigned int spaces;
	spaces = count_spaces(buffer);
	if(spaces % 2)
	{
		buffer->error = 1;
		return 0;
	}
	return spaces / 2;
}
static YAML* parse_sequence(PData *buffer)
{
	YAML *result;
	List *values;
	unsigned int level, position;

	values = init_list();

	while(*(buffer->str))
	{
		level = get_current_level(buffer);
		if(buffer->error != 2 && buffer->error > 0)
		{
			/*delete_list(values);*/
			return NULL;
		}
		if(buffer->str[level*2] != '-')
		{
			buffer->error = 0;
			result = malloc(sizeof(YAML));
			result->type = type_list;
			result->value.sequence = values;
			return result;
		}

		if(buffer->error == 0)
		{
			position = level * 2;
			buffer->str[position] = ' ';
			buffer->level++;
			/* assume that every list consist of objects */
			result = parse_object(buffer);
			if(buffer->error == 2)
			{
				buffer->level--;
				result = get_value(buffer);
			}
			list_append(values, result);
			continue;
		}
		buffer->str += level * 2 + 2;
		result = get_value(buffer);
		/*printf("list value %s\n", result->value.literal);*/
		list_append(values, result);
	}
	
	return NULL;
}


static YAML* parse_object(PData *buffer)
{
	char *key;
	YAML *field, *result;
	unsigned int level;

	result = malloc(sizeof(YAML));
	result->value.object = init_hmap();
	result->type = type_object;

	while(*(buffer->str) && buffer->error == 0)
	{
		level = get_current_level(buffer);
		if(buffer->error)
			return NULL;

		if(level != buffer->level)
			break;

		/* skip first identation spaces */
		buffer->str += buffer->level * 2;

		key = get_key(buffer);
		/*if(level == 0)
			printf("yaml key %s\n", key);*/
		if(buffer->error)
			return NULL;

		/*if(i > 49570)
			printf("near end\n");*/
		/*printf("key %s level %u\n", key, buffer->level);*/
		field = get_value(buffer);
		hmap_insert(result->value.object, key, field);
	}
	buffer->level--;
	return result;
}


