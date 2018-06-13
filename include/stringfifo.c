#include<stddef.h>
#include<stdlib.h>
#include "./stringfifo.h"

StringFifo *createStringFifo() {
	return (StringFifo*) malloc(sizeof(StringFifo));
}

void destroyStringFifo(StringFifo* fifo) {
	if(!fifo) return;

	while(hasNext(fifo)) {
		free(getString(fifo));
	}

	free(fifo);
}

char* getString(StringFifo *fifo) {
	StringElem *e = fifo->elem;

	fifo->elem = fifo->elem->next;

	char* s = e->content;
	free(e);
	return s;
}

int addString(char *string, StringFifo *fifo) {
	if(!string || !fifo) return 0;		

	StringElem *e;
	if(hasNext(fifo)) {
		e = fifo->elem;
		while(e->next) e = e->next;
		e = e->next;
		e->next = (StringElem*) malloc(sizeof(StringElem));
	}

	e = (StringElem*) malloc(sizeof(StringElem));
	fifo->elem = e;
	
	return e ? 1 : 0;
}

int hasNext(StringFifo *fifo) {
	return (fifo == NULL || fifo->elem == NULL) ? 0 : 1;
}


