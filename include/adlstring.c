#include"adlstring.h"
#include"fstatemachine.h"
#include"stringstack.h"
#include<string.h>
#include<stdio.h>
#include<stdlib.h>

#define EDGE_MALLOC_CHUNK 20

int isPartOfGraph(Graph *g, unsigned int index) {
	if(index >= 0 && index < g->edgeAmount) {
		return 1;
	} else {	
		return 0;
	}
}

int hasFollower(Graph *g, unsigned int index) {
	if(g->N[index] != NULL) {
		return 1;
	} else {
		return 0;
	}
}

char* getEdgeName(Graph *g, unsigned int index) {
		if(index < g->edgeAmount && index >= 0) {
			return g->A[index];
		} else {
			return NULL;
		}
}

int insertEdge(Graph *g, unsigned int predIndex, unsigned int newIndex) {
		if(!isPartOfGraph(g, predIndex) || !isPartOfGraph(g, newIndex)) {
			return 0;
		}

		if(hasFollower(g, predIndex)) {
			Edge *e = g->N[predIndex];
			
			while(e->succ != NULL) {
				e = e->succ;
			}

			e->succ = (Edge*) malloc(sizeof(Edge));
			
			if(e->succ == NULL) {
				puts("Konnte keinen Speicher für nachfolgende Ecke allozieren");
				exit(1);
			}	

			e->succ->index = newIndex;
			e->succ->pred = e;
		} else {
			g->N[predIndex] = (Edge*) malloc(sizeof(Edge));
		
			if(g->N[predIndex] == NULL) {
				puts("Konnte keinen Speicher für nachfolgende Ecke allozieren");
				exit(1);
			}	

			g->N[predIndex]->index = newIndex;
			g->N[predIndex]->pred = NULL;
			g->N[predIndex]->succ = NULL;
		}

		return 1;
}	

/* ÜBERPRÜFE OB DIE ECKE BEREITS ERSTELLT 
 * GIBT DEN INDEX DER ECKE ZURÜCK
 * GIBT -1 ZURÜCK FALLS ECKE NOCH NICHT VORHANDEN */
int hasEdge(Graph *g, char *edgeName) {
	for(unsigned int i = 0; i < g->edgeAmount; i++) {
		if(strcmp(g->A[i], edgeName) == 0) {
			return i;
		}	
	}

	return -1; 	
}

/* ERSTELLE EINE ECKE IM ÜBERGEBENEN GRAPHEN,
 * FALLS NOCH KEINE ECKE MIT DIESEM NAMEN VORHANDEN IST
 * GIBT DEN INDEX DER NEU ERSTELLTEN, ODER GEFUNDENEN ECKE ZURÜCK
 */
int createEdge(Graph *g, char *edgeName) {
	unsigned int memSize; 
	int edgeIndex = hasEdge(g, edgeName);
	
	if(edgeIndex < 0) {
		if(g->edgeAmount % (unsigned int) EDGE_MALLOC_CHUNK == 0) {
			memSize = g->edgeAmount + (unsigned int) EDGE_MALLOC_CHUNK;
			g->A = (char**) realloc(g->A, memSize * sizeof(char**)); 
			g->N = (Edge**) realloc(g->N, memSize * sizeof(Edge**));

			if(g->A == NULL || g->N == NULL) {
				puts("Fehler beim allozieren von Speicher. Terminiere.");
				exit(1);
			}	
		}
		edgeIndex = g->edgeAmount;

		g->A[edgeIndex] = (char*) malloc(strlen(edgeName) * sizeof(char));
		strcpy(g->A[edgeIndex], edgeName);
		g->edgeAmount++;

		g->N[edgeIndex] = NULL; 
	}

	return edgeIndex;
}

typedef struct voidParams {
		
} VoidParams;

typedef struct createEdgeParams {
	Graph *graph;
	FStateMachine *machine;
} CreateEdgeParams;

typedef struct buildStackParams {
	FStateMachine *machine;
	StringStack *stack;
} BuildStackParams;

void buildStack(void* params) {
	StringStack *stack = ((BuildStackParams)params)->stack;
	FStateMachine *machine = ((BuildStackParams)params)->machine;	

	if(!hasPlace(stack)) {
		stack = enlargeStringStack(stack, stack->size);
		((BuildStackParams)params)->stack = stack;
	}

	pushString(stack, machine->lastInput);
}

void createEdge(void* params) {
	StringStack *stack = ((CreateEdgeParams)params)->stack;
	FStateMachine *machine = ((CreateEdgeParams)params)->machine;
	Graph *graph = ((CreateEdgeParams)params)->graph;

	char* edgeName = stackToString(stack);
	createEdge(graph, edgeName); 
}

int onEntryCreate(void* params) {
	Graph *g = ((CreateEdgeParams*)params)->graph;
	FStateMachine *machine = ((CreateEdgeParams*)params)->machine;

	unsigned int index = createEdge(g, machine->lastInput);
	g->nextN = index;
	return 1;
}

int onEntryCreateAndInsert(void* params) {
	CreateEdgeParams *param = (CreateEdgeParams*) params;
	Graph *g = param->graph;
	FStateMachine *machine = param->machine;

	int newIndex = createEdge(g, machine->lastInput);
	insertEdge(g, g->nextN, newIndex);	

	return 1;
}

unsigned int nextCharacter(const char* string) {
	unsigned char firstByte = (unsigned char) *string;

	if(firstByte >= 240) {
		return 4;
	} else if(firstByte >= 224 && firstByte < 240) {
		return 3;
	} else if(firstByte >= 192 && firstByte < 224) {
		return 2;
	} else if(firstByte < 192 && firstByte > 0) {
		return 1;
	} else {
		return 0;
	}
}

void printMachine(FStateMachine *machine) {
	unsigned int nrStates = machine->numberOfStates;
	State *state;
	Transition *transitions;

	for(unsigned int i = 0; i < nrStates; i++) {
		state = &machine->states[i];
		printf("State %i: \n", state->index);
		printf("%i Transitions: \n", state->numberFollowingStates);
		transitions = state->transitions;

		for(unsigned int j = 0; j < state->numberFollowingStates; j++) {
			printf("over %s to %i\n", transitions[j].input, transitions[j].followingState);
		}

		printf("Default transition: %i\n", state->defaultTransition);
		printf("Is final: %i\n", state->isFinal);

		printf("Entryfunction: %p\n", state->entryFunction);
		printf("EntryParams: %p\n", state->entryFunctionParam);

		printf("\n");
	}
}


FStateMachine *buildMachine(Graph *g) {
	FStateMachine *machine = createMachine(8);

	if(machine == NULL) {
		return NULL;
	}

	void* createEdgeParams = (void*)malloc(sizeof(CreateEdgeParams));
	((CreateEdgeParams*)createEdgeParams)->graph = g;
	((CreateEdgeParams*)createEdgeParams)->machine = machine;

	defineAsFinal(6,machine);
	defineAsError(7,machine);

	defineDefaultTransition(0, 1, machine);
	defineTransition(0, ":", 7, machine);
	defineTransition(0, ";", 7, machine);
	defineTransition(0, ",", 7, machine);

	defineDefaultTransition(1, 1, machine);
	defineTransition(1, ":", 2, machine);
	defineTransition(1, ",", 7, machine);
	defineTransition(1, ";", 7, machine);

	defineDefaultTransition(2, 3, machine);
	defineTransition(2, ";", 5, machine);
	defineTransition(2, ",", 7, machine);
	defineTransition(2, ":", 7, machine);

	defineDefaultTransition(3, 3, machine);
	defineTransition(3, ",", 4, machine);
	defineTransition(3, ":", 7, machine);
	defineTransition(3, ";", 5, machine);

	defineDefaultTransition(4, 3, machine);
	defineTransition(4, ",", 7, machine);
	defineTransition(4, ";", 7, machine);
	defineTransition(4, ",", 7, machine);

	defineDefaultTransition(5, 1, machine);
	defineTransition(5, ";", 6, machine);
	defineTransition(5, ",", 7, machine);
	defineTransition(5, ":", 7, machine);

	defineDefaultTransition(6, 7, machine);	

	return machine;
}


void printGraph(Graph *g) {
	printf("Edgeamount: %i\n", g->edgeAmount);
	Edge *edge;
	for(unsigned int i = 0; i < g->edgeAmount; i++) {
		printf("Edge %s is followed by: \n", getEdgeName(g,i));
		if(hasFollower(g, i)) {
			edge = g->N[i];
		} else {
			continue;
		}
		while(edge->succ != NULL) {
			printf("%s, ", getEdgeName(g, edge->index));
			edge = edge->succ;
		}

		printf("%s",getEdgeName(g, edge->index));
		printf("\n");	
	}
}

Graph *generateGraph(const char *adlstring) {
	Graph *g = malloc(sizeof(Graph));

	if(g == NULL) {
		puts("Konnte keinen Speicher anfordern");
		exit(1);
	}

	g->A = (char**) malloc(EDGE_MALLOC_CHUNK * sizeof(char**));
	g->N = (Edge**) malloc(EDGE_MALLOC_CHUNK * sizeof(Edge**));	
	g->edgeAmount = 0;
	g->nextN = 0;

	FStateMachine *machine = buildMachine(g);

	if(machine == NULL) {
		puts("Konnte Automaten nicht bauen");
		exit(1);
	}

	char terminatedChar[5];
	const char *ptr = adlstring;
	unsigned int byteLength = nextCharacter(ptr);
	printf("%i\n", byteLength);

	while(byteLength) {
		memset(terminatedChar, 0, 5);
		strncpy(terminatedChar, ptr, byteLength);

		computeInput(terminatedChar, machine);

		ptr += byteLength;
		byteLength = nextCharacter(ptr);

		if(inError(machine)) {
			puts("Parsing and building failed. Ended in errorstate");
			printf("Unexpected %s",terminatedChar);
			destroyMachine(machine);
			break;
		} else if(inAccepted(machine)) {
			puts("Parsing and building complete. Ended in accepted state. Your Graph is ready");
			destroyMachine(machine);
			break;
		}
	}

	return g;
}

const char *readStringFromFile(const char *filename) {
	unsigned int chunksize = 4;
	unsigned int chunkInBytes = chunksize * sizeof(unsigned int);
	char buffer[chunksize];
	FILE *fd = fopen(filename ,"r");

	if(fd == NULL) {
		return NULL;
	}

	char *adjString = (char*) malloc(chunkInBytes);
	
	if(adjString == NULL) {
		return NULL;
	}

	unsigned int iteration = 1;
	
	while(fgets(buffer, chunksize, fd) != NULL) {
		iteration++;
		adjString = (char *) realloc(adjString, iteration * chunkInBytes);

		if(adjString == NULL) {
			return NULL;
		}

		strcat(adjString, buffer);
	}

	fclose(fd);
	return adjString;
}
