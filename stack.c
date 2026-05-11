#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "stack.h"

void initStack(StackNode** top) {
    *top = NULL;
}

void pushHistory(StackNode** top, Flight* flight, int runway, int actionType) {
    StackNode* newNode = (StackNode*)malloc(sizeof(StackNode));
    
    // IMPORTANT: Create a DEEP COPY of the flight
    // This ensures history is independent of current state
    newNode->flight = (Flight*)malloc(sizeof(Flight));
    memcpy(newNode->flight, flight, sizeof(Flight));
    newNode->flight->next = NULL;
    
    newNode->runwayAssigned = runway;
    newNode->actionType = actionType;
    newNode->next = *top;
    *top = newNode;
}

Flight* popHistory(StackNode** top, int* runway, int* actionType) {
    if (*top == NULL) {
        return NULL;
    }
    
    StackNode* temp = *top;
    Flight* flight = temp->flight;
    *runway = temp->runwayAssigned;
    *actionType = temp->actionType;
    *top = (*top)->next;
    free(temp);
    return flight;
}

int isStackEmpty(StackNode* top) {
    return top == NULL;
}

void freeStack(StackNode** top) {
    while (!isStackEmpty(*top)) {
        int r, a;
        Flight* f = popHistory(top, &r, &a);
        freeFlight(f);
    }
}