#include "mystring.h"

static int define_sign(char *str, int *start);

char* file_to_str(const char *fn)
{
	FILE *ifs;
	int length;
	char *str;
	ifs = fopen(fn, "rb");
	if(!ifs)
	{
		fprintf(stderr, "Can't open the file");
		return NULL;
	}
	fseek(ifs, 0, SEEK_END);
	length = ftell(ifs);
	fseek(ifs, 0, SEEK_SET);
	str = malloc(sizeof(char) * (length + 1));
	fread(str, sizeof(char), length, ifs);
	str[length] = 0;
	fclose(ifs);
	return str;
}

int str_to_int(char *str)
{
	int step, sign, start;
	int sum = 0;

	sign = define_sign(str, &start);
	for(int i = start; str[i]; i++)
	{
		step = str[i] - '0';
		if(step >= 0 && step <= 9)
		{
			sum = sum * 10 + step;
		}
		else
		{
			return 0;
		}
	}
	return sum * sign;
}

static int define_sign(char *str, int *start)
{
	int s;
	if(*str == '-')
	{
		*start = 1;
		s = -1;
	}
	else if(*str >= '0' && *str <= '9')
	{
		*start = 0;
		s = 1;
	}
	else if(*str == '+')
	{
		*start = 1;
		s = 1;
	}
	else
	{
		s = 0;
		*start = 0;
	}
	return s;
}

char *int_to_str(int num)
{
	char *str;
	int length = 0;
	int num_copy = num;

	while(num_copy > 0)
	{
		num_copy /= 10;
		length++;
	}
	str = malloc(sizeof(char) * (length+1));
	for(int i = length; i > 0; i--)
	{
		str[i-1] = num % 10 + '0';
		num /= 10;
	}
	str[length] = 0;
	return str;
}


char* concat(char *dest, const char *src)
{
	size_t length;
	length = strlen(dest) + strlen(src);
	dest = realloc(dest, sizeof(char) * (length+1));
	strcat(dest, src);
	return dest;
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

unsigned int word_counter(const char *str, char separator)
{
	/*prev - is previous char whitespace*/
	bool wsp, prev;
	size_t i;
	unsigned int n;
	prev = false;
	for(i = 0, n = 0; str[i]; i++)
	{
		wsp = (str[i] == separator);
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
}

size_t read_until(const char *str, const char *limeters)
{
	size_t i;
	for(i = 0; str[i]; i++)
	{
		if(is_whitespace(str[i], limeters))
			break;
	}
	return i;
}

char** mysplit(const char *str, char separator, unsigned int *return_size)
{
	char **result;
	char *str_separator;
	size_t word_length, i;
	unsigned int n, j;
	n = word_counter(str, separator);
	/*printf("words count in split %d\nstring length %d\n", n, strlen(str));*/
	result = malloc(sizeof(char*)*n);

	str_separator = malloc(sizeof(char) * 2);
	str_separator[0] = separator;
	str_separator[1] = 0;

	for(i = 0, j = 0; j < n; i++)
	{
		/*printf("%c %d\n", str[i], str[i]);*/
		if(str[i] != separator)
		{
			word_length = read_until(str+i, str_separator);
			result[j] = malloc(sizeof(char) * (word_length+1));
			/*printf("end of string %s\ni %d\nword length %d\n", str+i, i, length);*/
			strncpy(result[j], str+i, word_length);
			result[j][word_length] = 0;
			i += word_length;
			j++;
		}
	}
	*return_size = n;
	return result;
}
