#include "./stringbuffer.h"
#include<stdlib.h>
#include<string.h>
#include<stdio.h>
StringBuffer* createStringBuffer(unsigned int size) {
	StringBuffer* sbuffer = (StringBuffer*) malloc(sizeof(StringBuffer));
	sbuffer->buffer = (char**) malloc(size*sizeof(char*));
	sbuffer->size = size;
	sbuffer->head = 0;

	return sbuffer;
}

void destroyStringBuffer(StringBuffer* sbuffer) {
	for(unsigned int i = 0; i < sbuffer->size; i++) {
		free(sbuffer->buffer[i]);
	}

	free(sbuffer);
}

char* addString(StringBuffer* sbuffer) {
	if(sbuffer->head == 0) return NULL;
	
	return sbuffer->buffer[sbuffer->head--];	
}

void getString(StringBuffer* sbuffer, char* string) {
	if(!hasSpace(sbuffer)) return;

	sbuffer->buffer[(sbuffer->head)++] = string;
}

int hasSpace(StringBuffer* sbuffer) {
	if(sbuffer->head < sbuffer->size) {
		return 1;
	} else {
		return 0;
	}
}

unsigned int getOverallStringSize(StringBuffer* sbuffer) {
	unsigned int size = 0;

	printf("head %i\n",sbuffer->head);
	for(unsigned int i = 0; i < sbuffer->head; i++) {
		size += strlen(sbuffer->buffer[i]);
	}

	return size;
}

char* bufferToString(StringBuffer* sbuffer) {
	unsigned int overallSize = 0;
	char *string = NULL;
	char *tmp = NULL;

	if(sbuffer->head == 0) {
		return NULL;
	} else if(sbuffer->head == 1) {
		return popString(sbuffer);
	}

	overallSize = getOverallStringSize(sbuffer);
	string = (char*) malloc(overallSize*sizeof(char) + sizeof(char));

	
	for(unsigned int i = 0; i < sbuffer->head; i++) {
		strcat(string, sbuffer->buffer[i]);
	}	

	return string;
}

StringBuffer* enlargeStringBuffer(StringBuffer* sbuffer, unsigned int enlargement) {
	sbuffer = (StringBuffer*) realloc(sbuffer, ((sbuffer->size)+enlargement)*sizeof(StringBuffer));
	sbuffer->size = sbuffer->size+enlargement;
	return sbuffer;
}



