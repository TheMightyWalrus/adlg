#include<stdlib.h>
#include<stdio.h>
#include<string.h>

#include"fstatemachine.h"

typedef struct voidparams {

} VoidParams;

VoidParams voidParams;

int voidEntry(void* voidParams) {
	return 1;
}

int isValidState(unsigned int index, FStateMachine *machine) {
	unsigned int numberOfStates = machine->numberOfStates;
	if(index >= 0 && index < numberOfStates) {
		return 1;
	} else {
		return 0;
	}
}

int isValidTransition(unsigned int fromIndex, unsigned int toIndex, FStateMachine *machine) {
	if(!isValidState(fromIndex, machine) || !isValidState(toIndex, machine)) {
		return 0;
	} else {
		return 1;
	}
}

int defineAsError(unsigned int index, FStateMachine *machine) {
	if(!isValidState(index, machine)) {
		return 0;
	}

	State *state = &machine->states[index];
	machine->errorState = state;

	free(state->transitions);
	state->transitions = &voidTransition;
	state->transitions->input = "";
	state->transitions->followingState = index;

	state->defaultTransition = index;
	state->isFinal = 0;
	state->numberFollowingStates = 0;
	
	return 1;
}

int defineAsFinal(unsigned int index, FStateMachine *machine) {
	if(!isValidState(index, machine)) {
		return 0;
	} else {
		machine->states[index].isFinal = 1;
		return 1;
	}
}

int defineDefaultTransition(unsigned int fromIndex, unsigned int toIndex, FStateMachine *machine) {
	if(!isValidTransition(fromIndex, toIndex, machine)) {
		return 0;
	}

	State *state = &machine->states[fromIndex];
	state->defaultTransition = toIndex;

	return 1;
}

int defineTransition(unsigned int fromIndex, char* input, unsigned int toIndex, FStateMachine *machine) {
	if(!isValidTransition(fromIndex, toIndex, machine)) {
		return 0;
	}

	State *state = &machine->states[fromIndex];
	unsigned int memSize = 0;

	//Allozierungstrategie überdenken, wird unnötig oft ausgeführt, nicht wie gewollt wenn array voll
	if(state->numberFollowingStates % FOLLOWING_STATE_ARRAY_SIZE == 0) {
		memSize = (state->numberFollowingStates + FOLLOWING_STATE_ARRAY_SIZE) * sizeof(Transition);
		(state->transitions) = (Transition*) realloc(state->transitions, memSize);
	}

	if(state->transitions == NULL) {
		puts("Konnte Transitionsarray nicht vergrößern");
		exit(1);
	}

	state->transitions[state->numberFollowingStates].input = input;
	state->transitions[state->numberFollowingStates].followingState = toIndex;

	state->numberFollowingStates++;

	return 1;
}

int bindEntryFunction(unsigned int stateIndex, entryFunction entryFunction, void *param, FStateMachine *machine) {
	if(!isValidState(stateIndex, machine)){ 
		return 0;
	}

	if(entryFunction == NULL) {
		printf("entryfunction is null\n");
		return 0;
	}

	machine->states[stateIndex].entryFunction = entryFunction;
	machine->states[stateIndex].entryFunctionParam = param;

	return 1;
}

FStateMachine *createMachine(unsigned int numberOfStates) {
	FStateMachine *machine = (FStateMachine*) malloc(sizeof(FStateMachine));

	if(machine == NULL) {
		return NULL;
	}

	machine->lastInput = "";
	machine->numberOfStates = numberOfStates;
	machine->states = (State*) malloc(numberOfStates*sizeof(State));
	
	if(machine->states == NULL) {
		return NULL;
	}

	machine->currentState = &machine->states[0];
	machine->previousState = &machine->states[0];
	machine->errorState = &machine->states[numberOfStates-1];

	for(unsigned int i = 0; i < numberOfStates; i++) {
		machine->states[i].transitions = (Transition*) malloc(FOLLOWING_STATE_ARRAY_SIZE * sizeof(Transition));

		if(machine->states[i].transitions == NULL) {
			return NULL;
		}

		machine->states[i].index = i;
		machine->states[i].entryFunction = &voidEntry;
		machine->states[i].entryFunctionParam = (void*) &voidParams; 
		machine->states[i].numberFollowingStates = 0;
		machine->states[i].defaultTransition = numberOfStates-1;
		machine->states[i].isFinal = 0;
	}

	return machine;
}

int destroyMachine(FStateMachine *machine) {
	for(unsigned int i = 0; i < machine->numberOfStates-1; i++) {
		free(machine->states[i].transitions);
	} 

	free(machine->states);
	free(machine);

	return 1;
}


int computeInput(char* input, FStateMachine *machine) {
	for(unsigned int i = 0; i < machine->currentState->numberFollowingStates; i++) {
		if(!strcmp(input, machine->currentState->transitions[i].input)) {
			machine->previousState = machine->currentState;
			machine->currentState = &machine->states[machine->currentState->transitions[i].followingState];
			machine->lastInput = input;
			machine->currentState->entryFunction(machine->currentState->entryFunctionParam);
			return 1;
		}	
	}

	machine->previousState = machine->currentState;
	machine->currentState = &machine->states[machine->currentState->defaultTransition];
	machine->lastInput = input;
	machine->currentState->entryFunction(machine->currentState->entryFunctionParam);

	return 0;
}

int inError(FStateMachine *machine) {
	if(machine->currentState == machine->errorState) {
		return 1;
	} else {
		return 0;
	}
}

int inAccepted(FStateMachine *machine) {
	if(machine->currentState->isFinal) {
		return 1;
	} else {
		return 0;
	}	
}
