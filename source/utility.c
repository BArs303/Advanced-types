#include "utility.h"
#include <stdlib.h>
#include <string.h>

static unsigned int word_counter(char *str, char *delims);

void ptr_swap(void *arg1, void *arg2)
{
	void *tmp;
	tmp = arg1;
	arg1 = arg2;
	arg2 = tmp;
	return;
}

void passive_destruct(void *element, void *params)
{
	return;
}

bool is_whitespace(char c, const char *whitespaces)
{
	size_t i;
	for(i = 0; whitespaces[i]; i++)
	{
		if(c == whitespaces[i])
			return true;
	}
	return false;
}

char* file_to_str(const char *filename)
{
	FILE *ifs;
	long length;
	char *ret;
	size_t size;

	ifs = fopen(filename, "rb");
	if(!ifs)
	{
		perror("Can't open the file");
		return NULL;
	}

	fseek(ifs, 0, SEEK_END);
	length = ftell(ifs);
	fseek(ifs, 0, SEEK_SET);
	size = length;

	ret = malloc(sizeof(char) * (length+1));
	fread(ret, sizeof(char), size, ifs);
	ret[length] = 0;
	return ret;
}

char* int_to_str(unsigned int num)
{
	char *str;
	size_t length;
	unsigned int num_copy;

	num_copy = num;
	length = 0;
	while(num_copy > 0)
	{
		num_copy /= 10;
		length++;
	}
	str = malloc(sizeof(char) * (length+1));
	for(size_t i = length; i > 0; i--)
	{
		str[i-1] = num % 10 + '0';
		num /= 10;
	}
	str[length] = 0;
	return str;
}

char** mysplit(char *str, char separator, unsigned int *size)
{
    /*
	char **result;
	char separator_str[2];
	size_t word_length, i;
	unsigned int j;
	*size = word_counter(str, separator);

	separator_str[0] = separator;
	separator_str[1] = 0;

	result = malloc(sizeof(char*) * (*size));
	for(i = 0, j = 0; j < *size; i++)
	{
		if(str[i] != separator)
		{
			word_length = strcspn(str+i, separator_str);
			result[j] = malloc(sizeof(char) * (word_length+1));
			strncpy(result[j], str+i, word_length);
			result[j][word_length] = 0;
			i += word_length;
			j++;
		}
	}
	return result;*/
    return NULL;
}

void print_buffer(void *buffer, size_t size)
{
	size_t i;
	char *value;
	value = buffer;
	for(i = 0; i < size; i++)
	{
		printf("byte %zu value %hhu hex value %hhX\n", i, value[i], value[i]);
	}
}

bool is_prime(int number)
{
    int i;
    if(number == 1 || number == 0)
        return true;
    for(i = 2; i <= number / 2; i++)
    {
        if(number % i == 0)
        {
            return false;
        }
    }
    return true;
}


char* str_duplicate(char *restrict str)
{
    char *result;
    size_t n;
    n = strlen(str) + 1;
    result = malloc(sizeof(char) * n);
    return strcpy(result, str);
}

static unsigned int word_counter(char *str, char *delims)
{
    /* is previous char a word char */
    bool prev;
    size_t i;
    unsigned int c;
    prev = false;
    for(i = 0, c = 0; str[i]; i++)
    {
        if(is_whitespace(str[i], delims) && prev)
        {
            c++;
            prev = false;
        }
        else if(is_whitespace(str[i], delims))
        {
            continue;
        }
        else
        {
            prev = true;
        }
    }
    return prev?c+1:c;
}

char** str_split(char *str, char *delims, unsigned int *size)
{
    unsigned int n, k;
    char *token, *str_copy;
    char **result;

    str_copy = str_duplicate(str);
    token = strtok(str_copy, delims);
    k = word_counter(str, delims);

    result = malloc(sizeof(char*) * k);
    *size = k;

    for(n = 0; token; n++)
    {
        if(n > k)
        {
            perror("word counter error\n");
            exit(EXIT_FAILURE);
        }
        result[n] = token;
        token = strtok(NULL, delims);
    }
    return result;
}

/* Old version 13.09.2025
 * unsigned int word_counter(char *str, char separator)
{
	//prev - is previous char whitespace*
	bool wsp, prev;
	size_t i;
	unsigned int n;

	prev = false;
	for(i = 0, n = 0; str[i]; i++)
	{
		wsp = str[i] == separator;
		if(wsp)
		{
			if(prev)
			{
				n++;
				prev = false;
			}
		}
		else
		{
			prev = true;
		}
	}
	if(prev)
		n++;
	return n;
}*/

