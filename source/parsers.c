#include "parsers.h"

const char default_whitespaces[] = {' ', '\n', '\t'};

size_t skip_whitespaces(PData *buffer, const char *whitespaces)
{
	char *position;
	position = buffer->str;
	while(*buffer->str && is_whitespace(*buffer->str, whitespaces))
	{
		buffer->str++;
	}
	return buffer->str - position;
}

char* cut_from_buffer(PData *buffer, size_t length)
{
	size_t i;
	char *ret;
	ret = malloc(sizeof(char) * (length + 1));
	for(i = 0; i < length; i++, buffer->str++)
	{
		ret[i] = *buffer->str;
	}
	ret[length] = 0;
	return ret;
}

char* my_concat(char *dst, const char *src)
{
	dst = realloc(dst, strlen(dst) + strlen(src) + 1);
	strcat(dst, src); 
	return dst;
}
