#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "flight.h"
// creating new flight in memory
Flight* createFlight(char* id, int fuel, char* type, int emergency) {
    Flight* newFlight = (Flight*)malloc(sizeof(Flight));
    strcpy(newFlight->flightID, id);  //copy the strings
    newFlight->fuelLevel = fuel;
    strcpy(newFlight->type, type);
    newFlight->emergency = emergency;
    newFlight->timestamp = 0;
    newFlight->next = NULL;
    newFlight->originalRunway = 0;
    newFlight->actionType = 0;
    return newFlight;
}

// Lower score = higher priority for runway assignment
int getPriorityScore(Flight* flight) {
    int score = 0;
    
    // Emergency flights get highest priority (negative score)
    if (flight->emergency) 
        score -= 1000;
    
    // Low fuel landings (below 20%) get very high priority
    if (strcmp(flight->type, "Landing") == 0 && flight->fuelLevel < 20) 
        score -= 500;
    
    // Lower fuel = higher priority
    score += (100 - flight->fuelLevel) * 2;
    
    // Older flights (waiting longer) get priority
    score += flight->timestamp * 10;
    
    return score;  // Most negative = highest priority
}

void printFlight(Flight* flight) {
    printf("%s | Fuel: %d%% | %s", 
           flight->flightID, flight->fuelLevel, flight->type);
    if (flight->emergency) 
        printf(" | *** EMERGENCY ***");
}

void freeFlight(Flight* flight) {
    if (flight != NULL)
        free(flight);
}