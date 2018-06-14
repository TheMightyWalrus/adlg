#include<stdio.h>

#include"stringstack.h"

int main(int argc, char** argv) {
	StringStack *stack = createStringStack(20);

	pushString(stack, "lol");
	pushString(stack, "lol");
	pushString(stack, "lol");
	
	puts("s");
	printf("%s\n",stackToString(stack));

	return 0;
}
