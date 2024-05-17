#include "set.h"

static bool check_existence(Set *a, void *element);
static int sort_criteria(void *arg1, void *arg2);

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

bool set_insert_check(Set *a, void *element, bool (*f)(Set *s, void* element))
{
    return s_set_insert(a, element, f, &sort_criteria);
}

bool set_insert_sort(Set *a, void *element, int (*f)(void *arg1, void *arg2))
{
    return s_set_insert(a, element, &check_existence, f);
}

bool s_set_insert(Set *a, void *element, bool (*check)(Set *s, void* element), int (*ssort)(void *arg1, void *arg2))
{
	int i;
	if(check(a, element))
	{
		for(i = 0; i < a->size && ssort(element, darray_at_pos(a, i)) > 0 ; i++)
		{}
		if(i == a->size)
			return darray_append(a, element);
		else
			return darray_insert(a, element, i);
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

static bool check_existence(Set *a, void *element)
{
	/*for(int i = 0; i < a->size; i++)
	{
		if(darray_at_pos(a, i) == element)
		{
			return false;
		}
	}
	return true;*/
	return !set_binary_search(a, element, &sort_criteria);
}

static int sort_criteria(void *arg1, void *arg2)
{
	return 1;
}



