#include "set.h"

static int sort_criteria(void *arg1, void *arg2);
static int set_search(Set *a, void *element, bool *flag, int (*ssort)(void *arg1, void *arg2));
static int set_search_place(Set *a, void *element, bool *flag, int (*ssort)(void *arg1, void *arg2));

Set *init_set()
{
	return init_darray();
}

Set *init_set_with_length(int length)
{
	return init_darray_with_length(length);
}

void destruct_set(Set *a, void (*free_element)(void* element))
{
	destruct_darray(a, free_element);
}

void print_set(Set *a, void (*print_element)(void* element))
{
	print_darray(a, print_element);
}

void set_clear(Set *a, void (*free_element)(void* element))
{
	darray_clear(a, free_element);
}

bool set_insert(Set *a, void *element)
{
	return s_set_insert(a, element, &check_existence, &sort_criteria);
}

<<<<<<< HEAD
bool s_set_insert(Set *a, void *element, int (*ssort)(void *arg1, void *arg2))
{
	int index;
	bool flag;
	index = set_search_place(a, element, &flag, ssort);
	if(index >= 0 && index < a->size && !flag)
	{
		return darray_insert(a, element, index);
	}
	else if(!flag)
	{
		return darray_append(a, element);
	}
	return false;

}

bool set_remove(Set *a, int index, void (*free_element)(void* element))
{
	return darray_remove(a, index, free_element);
}

void* set_at_pos(Set *a, int index)
{
	return darray_at_pos(a, index);
}
void* set_binary_search(Set *a, void *element, int (*ssort)(void *arg1, void *arg2))
{
    return set_at_pos(a, set_search_index(a, element, ssort));
}

int set_search_index(Set *a, void *element, int (*ssort)(void *arg1, void *arg2))
{
	void *mitem;
	int start, end, middle;

	start = 0;
	end = a->size-1;
	while(start <= end)
	{
		middle = (start+end)/2;
		mitem = set_at_pos(a, middle);
		if(ssort(mitem, element) == 0)
		{
			return middle;
		}
		else if(ssort(mitem, element) < 0)
		{
			start = middle + 1;
		}
		else
		{
			end = middle - 1;
		}
	}
	return -1;
}

void* set_binary_search(Set *a, void *element, int (*ssort)(void *arg1, void *arg2))
{
	int index;
	bool flag;
	index = set_search(a, element, &flag, ssort);
	if(flag)
	{
		return set_at_pos(a, index);
	}
	return NULL;
}

static int set_search(Set *a, void *element, bool *flag, int (*ssort)(void *arg1, void *arg2))
{
	void *mitem;
	int start, end, middle;

	start = 0;
	*flag = false;
	end = a->size-1;
	while(start<=end)
	{
		middle = (start+end)/2;
		mitem = set_at_pos(a, middle);
		if(ssort(mitem, element) == 0)
		{
			*flag = true;
			return middle;
		}
		else if(ssort(mitem, element) < 0)
		{
			start = middle + 1;
		}
		else
		{
			end = middle - 1;
		}
	}
	return end+1;

}

bool check_existence(Set *a, void *element, int (*ssort)(void *arg1, void *arg2))
{
	bool flag;
	set_search(a, element, &flag, ssort);
	return flag;
}

int set_search_index(Set *a, void *element, int (*ssort)(void *arg1, void *arg2))
{
	int index;
	bool flag;
	index = set_search(a, element, &flag, ssort);
	if(flag)
	{
		return index;
	}
	return -1;
}


static int set_search_place(Set *a, void *element, bool *flag, int (*ssort)(void *arg1, void *arg2))
{
	int index;
	index = set_search(a, element, flag, ssort);
	return index;
}

static int sort_criteria(void *arg1, void *arg2)
{
	return 1;
}	
