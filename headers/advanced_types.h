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

#include "dlinked_list.h"
#include "dynamic_array.h"
#include "set.h"

#define TYPE_STRING 1
#define TYPE_INT 2
#define TYPE_FLOAT 3
#define TYPE_STRUCT 4
#define TYPE_LIST 5

#endif
