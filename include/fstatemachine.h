#ifndef FSTATEMACHINE_H
#define FSTATEMACHINE_H

#define FOLLOWING_STATE_ARRAY_SIZE 3 

typedef int (*entryFunction)(void*);

int voidEntry(void*);

typedef struct transition {
	char* input;
	unsigned int followingState;
} Transition;

Transition voidTransition;

typedef struct state {
	unsigned int index;
	entryFunction entryFunction;
	void* entryFunctionParam;
	Transition *transitions;
	unsigned int defaultTransition;
	unsigned int numberFollowingStates;
	short isFinal;
} State;

typedef struct fsmachine {
	char* lastInput;
	unsigned int numberOfStates;
	State *states;
	State *currentState;
	State *previousState;
	State *errorState;
} FStateMachine;

int defineAsError(unsigned int, FStateMachine*);

int defineAsFinal(unsigned int, FStateMachine*);

int defineTransition(unsigned int, char*, unsigned int, FStateMachine*);

int defineDefaultTransition(unsigned int, unsigned int, FStateMachine*);

int bindEntryFunction(unsigned int, entryFunction, void*, FStateMachine*); 

/*
 * erstellt finite state machine
 * param0: anzahl der zustande
 */
FStateMachine* createMachine(unsigned int);

int destroyMachine(FStateMachine*);

int computeInput(char*, FStateMachine*);

int inError(FStateMachine*);

int inAccepted(FStateMachine*);

#endif
