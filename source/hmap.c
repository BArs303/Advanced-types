#include "set.h"

int function_hmap(const char *text){
	int sum = 0;
	for(int i = 0; text[i]; i++){
		sum += text[i];
	}
	return sum%7;
}
