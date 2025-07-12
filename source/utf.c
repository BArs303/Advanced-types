#include "utf.h"

static size_t get_utf_char_length(unsigned int code);
static size_t count_bytes(char *utf_char);

char* get_bytes(unsigned int code)
{
	char *result;
	char payload_header, pattern;
	size_t length;
	/*size_t debug_length;*/
	/* 
	 * 6 bits with ones
	 */
	pattern = 0x3F; 
	payload_header = 0x80;

	if(code >= 0x80)
	{
		length = get_utf_char_length(code);
		/*debug_length = length;*/
		result = malloc(sizeof(char) * length);
		result[0] = payload_header >> (length - 1);
		while(length > 1)
		{
			result[length - 1] = (code & pattern) | payload_header;
			code >>= 6;
			length--;
		}
		result[0] |= code;
	}
	else
	{
/*		debug_length = 1;*/
		result= malloc(sizeof(char));
		result[0] = (char)code;
	}
	/*for(size_t i = 0; i < debug_length; i++)
	{
		printf("%X\n", result[i]);
	}*/
	return result;	
}

unsigned int get_code(char *utf_char)
{
	int i;
	unsigned int result;
	/* mask bytes need to be signed for correct byte shifting*/
	char hmask, pmask;
	size_t length;

	pmask = 0x3F; 
	hmask = 0x80;
	length = count_bytes(utf_char);

	if(length > 0)
	{
		if(length > 1)
			hmask >>= length;
		result = utf_char[0] & ~hmask;

		for(i = 1; i < length; i++)
		{
			result <<= 6;
			result += utf_char[i] & pmask;
		}
	}
	else
	{
		/*invalid character. Return error code*/
		result = 0x80;
	}
	/*	hmask = 0x80 >> length;
	result = utf_char[0] & ~hmask;
	
	*/
	return result;
}
static size_t get_utf_char_length(unsigned int code)
{
	size_t result;
	char header;
	for
	(
		result = 0, header = 0x20;
		code >= header;
		code >>= 6, header >>= 1, result++
	)
	{}

	/* add header byte */
	result++;
	return result;
}

static size_t count_bytes(char *utf_char)
{
	unsigned char mask, header; 
	size_t result;

	mask = 0x80;
	header = *utf_char;
	result = 0;

	if(header >= mask)
	{
		while((mask & header) == mask)
		{
			mask >>= 1;
			result++;
		}
		if(result == 1)
			result = 0;
	}
	else
	{
		/*ascii character. size = 1 byte*/
		result = 1;
	}

	return result;
}
