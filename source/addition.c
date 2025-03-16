#include "advanced_types.h"

void ptr_swap(void **a, void **b)
{
	void *t;
	t = *a;
	*a = *b;
	*b = t;
	return;
}

void passive_destruct(void *element)
{
	return;
}
