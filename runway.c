#include <stdio.h>
#include <string.h>
#include "runway.h"

void initRunway(Runway* runway, char* name) {
    strcpy(runway->name, name);
    runway->currentlyProcessing = 0;
    runway->currentFlight = NULL;
}

int isRunwayIdle(Runway* runway) {
    return runway->currentlyProcessing == 0;
}

void assignToRunway(Runway* runway, Flight* flight) {
    runway->currentFlight = flight;
    runway->currentlyProcessing = 1;
}

void clearRunway(Runway* runway) {
    runway->currentFlight = NULL;
    runway->currentlyProcessing = 0;
}

void displayRunway(Runway* runway) {
    printf("Runway %s: ", runway->name);
    if (runway->currentlyProcessing) {
        printFlight(runway->currentFlight);
    } else {
        printf("IDLE");
    }
    printf("\n");
}