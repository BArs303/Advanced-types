#include "mystring.h"

static int define_sign(char *str, int *start);

char* file_to_str(const char *fn)
{
	FILE *ifs;
	int length;
	char *str = NULL;
	ifs = fopen(fn, "rb");
	if(ifs)
	{
		fseek(ifs, 0, SEEK_END);
		length = ftell(ifs);
		fseek(ifs, 0, SEEK_SET);
		str = malloc(sizeof(char) * length);
		fread(str, sizeof(char), length, ifs);
		fclose(ifs);
	}
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
	}
	str[length] = 0;
	return str;
}
