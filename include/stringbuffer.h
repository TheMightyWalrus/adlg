#ifndef STRINGBUFFER_H 
#define STRINGBUFFER_H

typedef struct bufferbuffer {
	unsigned int size;
	unsigned int head;
	char** buffer;
} StringBuffer;

char* bufferToString(StringBuffer*);

char* getString(StringBuffer*);

void addString(StringBuffer*, char*);

StringBuffer* createStringBuffer(unsigned int);

void destroyStringBuffer(StringBuffer*);

StringBuffer* enlargeStringBuffer(StringBuffer*, unsigned int);

int hasSpace(StringBuffer*);
#endif
