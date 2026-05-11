#ifndef SIMULATOR_H
#define SIMULATOR_H

#include "queue.h"
#include "stack.h"
#include "bst.h"
#include "runway.h"

// Global variables (declared as extern)
extern Queue landingQueue;
extern Queue takeoffQueue;
extern StackNode* historyStack;
extern BSTNode* flightArchive;
extern Runway runwayAlpha;
extern Runway runwayBravo;
extern int congestionMode;
extern int currentTimestamp;
extern int emergencyMode;

// Core simulation functions
void initSimulation();
void processTimeStep();
void assignFlightsToRunways();
void completeLanding(Runway* runway);
void completeTakeoff(Runway* runway);
void processEmergency();
void undoLastAssignment();
void displayStatistics();

#endif