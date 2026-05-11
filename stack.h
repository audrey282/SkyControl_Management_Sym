#ifndef STACK_H
#define STACK_H

#include "flight.h"

typedef struct StackNode {
    Flight* flight;          // Deep copy of flight
    int runwayAssigned;      // 1=Alpha, 2=Bravo
    int actionType;          // 0=assigned, 1=completed
    struct StackNode* next;
} StackNode;

// Stack operations
void initStack(StackNode** top);
void pushHistory(StackNode** top, Flight* flight, int runway, int actionType);
Flight* popHistory(StackNode** top, int* runway, int* actionType);
int isStackEmpty(StackNode* top);
void freeStack(StackNode** top);

#endif