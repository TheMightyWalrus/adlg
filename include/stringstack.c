#include "./stringstack.h"
#include<stdlib.h>

StringStack* createStringStack(unsigned int size) {
	StringStack* sstack = (StringStack*) malloc(sizeof(StringStack));
	sstack->stack = (char**) malloc(size*sizeof(char*));
	sstack->size = size;
	sstack->head = 0;

	return sstack;
}

void destroyStringStack(StringStack* sstack) {
	for(unsigned int i = 0; i < sstack->size; i++) {
		free(sstack->stack[i]);
	}

	free(sstack);
}

char* popString(StringStack* sstack) {
	if(sstack->head == 0) return NULL;
	
	return sstack->stack[sstack->head--];	
}

void pushString(StringStack* sstack, char* string) {
	if(!hasSpace(sstack)) return;

	sstack->stack[++(sstack->head)] = string;
}

int hasSpace(StringStack* sstack) {
	if(sstack->head < sstack->size) {
		return 1;
	} else {
		return 0;
	}
}


StringStack* enlargeStringStack(StringStack* sstack, unsigned int enlargement) {
	sstack = (StringStack*) realloc(sstack, ((sstack->size)+enlargement)*sizeof(StringStack));
	sstack->size = sstack->size+enlargement;
	return sstack;
}





