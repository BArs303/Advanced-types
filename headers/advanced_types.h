#ifndef ADVANCEDT_LIB_H
#define ADVANCEDT_LIB_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifndef bool
#define bool char
#define false 0
#define true !0
#endif

#ifndef CAST
#define CAST(type, value) ((type)value)
#endif

enum te
{
	type_Int,
	type_String,
	type_Darray,
	type_List,
	type_Object,
	type_Bool,
	type_Error
};

typedef enum te Types;

/*Library headers*/
#include "dlinked_list.h"
#include "dynamic_array.h"
#include "queue.h"
#include "mystring.h"
#include "red_black_tree.h"
#include "rbt_set.h"
#include "hmap.h"
#include "parsers.h"

void ptr_swap(void **a, void **b);
void passive_destruct(void *element);
//#include "set.h" replaced

#endif
