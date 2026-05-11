#ifndef RUNWAY_H
#define RUNWAY_H

#include "flight.h"

typedef struct Runway {
    char name[10];          // "Alpha" or "Bravo"
    int currentlyProcessing; // 0 = idle, 1 = busy
    Flight* currentFlight;
} Runway;

// Runway operations
void initRunway(Runway* runway, char* name);
int isRunwayIdle(Runway* runway);
void assignToRunway(Runway* runway, Flight* flight);
void clearRunway(Runway* runway);
void displayRunway(Runway* runway);

#endif