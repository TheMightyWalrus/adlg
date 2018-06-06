#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include"./include/adlstring.h"

int main(int argc, char **argv) {
	const char *adjString;

	if(argc > 1) {
		argv++;
		adjString = *argv;
	} else {
		adjString = readStringFromFile("./adlfile");

		if(adjString == NULL) {
			printf("Die Adjazenzliste konnte nicht korrekt aus der Datei gelesen werden\n");
			exit(1);
		} 
	}

	generateGraph(adjString);
	//parse adjString -> erhalte A[], N[]

	//führe operationen aus

	return 0;	
}




