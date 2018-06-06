/* Enhält alle nötigen Datenstrukturen und Operationen um Adjazenzlisten auszulesen 
 * und zu verarbeiten 
 */

#ifndef ADLSTRING_H
#define ADLSTRING_H

typedef struct edge {
	unsigned int index;
	struct edge *pred;
	struct edge *succ;
} Edge;

typedef struct graph {
	Edge **N;
	char **A;
	unsigned int edgeAmount;
	unsigned int nextN;
} Graph;

const char *readStringFromFile(const char* fileName);

Graph *generateGraph(const char* adlString);

#endif


