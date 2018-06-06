/*
 * myGraph.h
 * Beinhaltet alle noetigen Funktionen und Datenstrukturen um einen Graphen aus
 * einem Adjazenzstring zu generieren
 */

#ifndef MYGRAPH_H
#define MYGRAPH_H

typedef struct follower {
	unsigned int index;
	struct follower next;
} Follower;

typedef struct graph {
	char A[];
	unsigned int edgeCount;
	Follower N[];
} Graph;

Graph *parse(char *string);

#endif
