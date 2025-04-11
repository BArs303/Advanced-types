#include "parsers.h"

static bool is_whitespace(char c, const char *whitespaces);

unsigned int yskip_whitespaces(PData *buffer, const char *whitespaces)
{
	char *position;
	position = buffer->str;
	while(*buffer->str && is_whitespace(*buffer->str, whitespaces))
	{
		buffer->str++;
	}
	return buffer->str - position;
}

char* ycut_from_buffer(PData *buffer)
{
	int i;
	char *str;
	str = malloc(sizeof(char) * buffer->lengthcpy + 1);
	for(i = 0; i < buffer->lengthcpy; i++, buffer->str++)
	{
		str[i] = *buffer->str;
	}
	str[buffer->lengthcpy] = 0;
	return str;
}

unsigned int yread_until(char *str, const char *limiters)
{
	int i, j;
	for(i = 0; str[i]; i++)
	{
		for(j = 0; limiters[j]; j++)
		{
			if(str[i] == limiters[j])
				return i;
		}
	}
	return 0;
}

static bool is_whitespace(char c, const char *whitespaces)
{
	int i;
	for(i = 0; whitespaces[i]; i++)
	{
		if(c == whitespaces[i])
			return true;
	}
	return false;
}
