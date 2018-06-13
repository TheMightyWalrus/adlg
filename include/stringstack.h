#ifndef STRINGSTACK_H
#define STRINGSTACK_H

typedef struct stringstack {
	unsigned int size;
	unsigned int head;
	char** stack;
} StringStack;

char* popString(StringStack*);

void pushString(StringStack*, char*);

StringStack* createStringStack(unsigned int);

void destroyStringStack(StringStack*);

StringStack* enlargeStringStack(StringStack*, unsigned int);

int hasSpace(StringStack*);
#endif
