#ifndef FLIGHT_H
#define FLIGHT_H

#include <stdio.h>   // For printf in printFlight

#define MAX_FLIGHT_ID 20

typedef struct Flight {
    char flightID[MAX_FLIGHT_ID];
    int fuelLevel;          // 0-100, lower = more urgent
    char type[10];          // "Landing" or "Takeoff"
    int emergency;          // 1 if emergency, 0 otherwise
    int timestamp;          // For tracking wait time
    struct Flight* next;    //pointer to another flight (it's not completed yet so struct....)
    int originalRunway;     // For undo (1=Alpha, 2=Bravo)
    int actionType;         // 0=assigned, 1=completed
} Flight;

// Function prototypes
Flight* createFlight(char* id, int fuel, char* type, int emergency);
int getPriorityScore(Flight* flight);
void printFlight(Flight* flight);
void freeFlight(Flight* flight);

#endif

//define flight structure, declare constants and function prototypes