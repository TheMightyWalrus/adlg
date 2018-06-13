#include <stdio.h>
#include "./stringfifo.h"

int main(int argc, char **argv) {
	StringFifo *fifo = createStringFifo();

	if(!fifo) puts("is null");

	

	return 0;
}
