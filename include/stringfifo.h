#ifndef STRINGFIFO_H
#define STRINGFIFO_H

typedef struct stringelem {
	char* content;
	struct stringelem* next;
	struct stringelem* prev;
} StringElem;

typedef struct stringfifo {
	StringElem* elem;
} StringFifo;

StringFifo *createStringFifo();

void destroyStringFifo(StringFifo*);

int addString(char*, StringFifo*);

char* getString(StringFifo*);

int hasNext(StringFifo*);

#endif
