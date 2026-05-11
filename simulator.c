#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "simulator.h"
#include "flight.h"

// Global variables definition
Queue landingQueue;
Queue takeoffQueue;
StackNode* historyStack;
BSTNode* flightArchive;
Runway runwayAlpha;
Runway runwayBravo;
int congestionMode;
int currentTimestamp;
int emergencyMode;

#define CONGESTION_THRESHOLD 5

void initSimulation() {
    initQueue(&landingQueue);
    initQueue(&takeoffQueue);
    initStack(&historyStack);
    flightArchive = NULL;
    initRunway(&runwayAlpha, "Alpha");
    initRunway(&runwayBravo, "Bravo");
    congestionMode = 0;
    currentTimestamp = 0;
    emergencyMode = 0;
}

void completeLanding(Runway* runway) {
    printf("\n  ✈️  Flight %s has LANDED on Runway %s", 
           runway->currentFlight->flightID, runway->name);
    if (runway->currentFlight->emergency) {
        printf(" [EMERGENCY RESOLVED]");
    }
    printf("\n");
    
    runway->currentFlight->timestamp = currentTimestamp;
    runway->currentFlight->actionType = 1;
    
    // Archive to BST
    flightArchive = insertBST(flightArchive, runway->currentFlight);
    
    // Record completion in history
    pushHistory(&historyStack, runway->currentFlight, 
                (strcmp(runway->name, "Alpha") == 0) ? 1 : 2, 1);
    
    free(runway->currentFlight);
    clearRunway(runway);
}

void completeTakeoff(Runway* runway) {
    printf("\n  🚀  Flight %s has TAKEN OFF from Runway %s\n", 
           runway->currentFlight->flightID, runway->name);
    
    runway->currentFlight->timestamp = currentTimestamp;
    runway->currentFlight->actionType = 1;
    
    // Archive to BST
    flightArchive = insertBST(flightArchive, runway->currentFlight);
    
    // Record completion in history
    pushHistory(&historyStack, runway->currentFlight, 
                (strcmp(runway->name, "Alpha") == 0) ? 1 : 2, 1);
    
    free(runway->currentFlight);
    clearRunway(runway);
}

void assignFlightsToRunways() {
    // Check congestion mode
    congestionMode = (landingQueue.size + takeoffQueue.size) > CONGESTION_THRESHOLD;
    
    printf("\n=== Runway Assignment (t=%d) ===\n", currentTimestamp);
    if (congestionMode) {
        printf("*** CONGESTION MODE ACTIVE ***\n");
    } else if (emergencyMode) {
        printf("*** EMERGENCY MODE ACTIVE ***\n");
    } else {
        printf("Normal Mode: Alpha=Landings, Bravo=Takeoffs\n");
    }
    
    // Runway Alpha Assignment
    if (isRunwayIdle(&runwayAlpha)) {
        Flight* next = NULL;
        
        if (emergencyMode) {
            // Find emergency landing
            QueueNode* curr = landingQueue.front;
            while (curr != NULL) {
                if (curr->flight->emergency) {
                    next = dequeue(&landingQueue);
                    break;
                }
                curr = curr->next;
            }
        } else if (!isQueueEmpty(&landingQueue)) {
            next = dequeue(&landingQueue);
        }
        
        if (next != NULL) {
            assignToRunway(&runwayAlpha, next);
            pushHistory(&historyStack, next, 1, 0);
            printf("  Runway Alpha assigned to ");
            printFlight(next);
            printf("\n");
        }
    }
    
    // Runway Bravo Assignment
    if (isRunwayIdle(&runwayBravo)) {
        Flight* next = NULL;
        
        if (!emergencyMode && !isQueueEmpty(&takeoffQueue)) {
            next = dequeue(&takeoffQueue);
        } else if (congestionMode && !isQueueEmpty(&landingQueue)) {
            next = dequeue(&landingQueue);
        }
        
        if (next != NULL) {
            assignToRunway(&runwayBravo, next);
            pushHistory(&historyStack, next, 2, 0);
            printf("  Runway Bravo assigned to ");
            printFlight(next);
            printf("\n");
        }
    }
}

void processTimeStep() {
    printf("\n=======================\n");
    printf("PROCESSING TIME STEP %d\n", currentTimestamp);
    printf("=======================\n");
    
    // Decrease fuel for waiting flights
    QueueNode* curr = landingQueue.front;
    while (curr != NULL) {
        if (curr->flight->fuelLevel > 0) {
            curr->flight->fuelLevel -= 5;
            if (curr->flight->fuelLevel < 0) curr->flight->fuelLevel = 0;
        }
        curr = curr->next;
    }
    
    // Complete current operations
    if (runwayAlpha.currentlyProcessing) {
        completeLanding(&runwayAlpha);
    }
    if (runwayBravo.currentlyProcessing) {
        if (strcmp(runwayBravo.currentFlight->type, "Landing") == 0) {
            completeLanding(&runwayBravo);
        } else {
            completeTakeoff(&runwayBravo);
        }
    }
    
    // Assign new flights
    assignFlightsToRunways();
    
    currentTimestamp++;
}

void processEmergency() {
    printf("\n*** MAYDAY - EMERGENCY PROTOCOL ***\n");
    
    // Find emergency flight in landing queue
    QueueNode* curr = landingQueue.front;
    QueueNode* prev = NULL;
    Flight* emergencyFlight = NULL;
    
    while (curr != NULL) {
        if (curr->flight->emergency) {
            emergencyFlight = curr->flight;
            // Remove from queue
            if (prev == NULL) {
                landingQueue.front = curr->next;
            } else {
                prev->next = curr->next;
            }
            if (curr == landingQueue.rear) {
                landingQueue.rear = prev;
            }
            free(curr);
            landingQueue.size--;
            break;
        }
        prev = curr;
        curr = curr->next;
    }
    
    if (emergencyFlight != NULL) {
        printf("Emergency flight %s fast-tracked!\n", emergencyFlight->flightID);
        
        // Clear Runway Alpha if busy
        if (runwayAlpha.currentlyProcessing) {
            Flight* bumped = runwayAlpha.currentFlight;
            enqueuePriority(&landingQueue, bumped);
            printf("  Flight %s returned to queue\n", bumped->flightID);
        }
        
        // Assign emergency to Alpha
        assignToRunway(&runwayAlpha, emergencyFlight);
        pushHistory(&historyStack, emergencyFlight, 1, 0);
        emergencyMode = 1;
        printf("EMERGENCY MODE ACTIVATED\n");
    } else {
        printf("No emergency flights in queue.\n");
    }
}

void undoLastAssignment() {
    int runway, actionType;
    Flight* undoneFlight = popHistory(&historyStack, &runway, &actionType);
    
    if (undoneFlight == NULL) {
        printf("\n*** No actions to undo ***\n");
        return;
    }
    
    printf("\n=== UNDO: %s of %s on Runway %s ===\n",
           (actionType == 0) ? "assignment" : "completion",
           undoneFlight->flightID, (runway == 1) ? "Alpha" : "Bravo");
    
    if (actionType == 0) {
        // Undo assignment
        if (strcmp(undoneFlight->type, "Landing") == 0) {
            enqueuePriority(&landingQueue, undoneFlight);
        } else {
            enqueuePriority(&takeoffQueue, undoneFlight);
        }
        
        // Clear runway if needed
        if (runway == 1 && runwayAlpha.currentlyProcessing &&
            strcmp(runwayAlpha.currentFlight->flightID, undoneFlight->flightID) == 0) {
            clearRunway(&runwayAlpha);
        } else if (runway == 2 && runwayBravo.currentlyProcessing &&
                   strcmp(runwayBravo.currentFlight->flightID, undoneFlight->flightID) == 0) {
            clearRunway(&runwayBravo);
        }
    } else {
        // Undo completion
        flightArchive = deleteBST(flightArchive, undoneFlight->flightID);
        
        if (strcmp(undoneFlight->type, "Landing") == 0) {
            enqueuePriority(&landingQueue, undoneFlight);
        } else {
            enqueuePriority(&takeoffQueue, undoneFlight);
        }
    }
}

void displayStatistics() {
    printf("\n=== SYSTEM STATISTICS ===\n");
    printf("Time: t=%d | Congestion: %s | Emergency: %s\n",
           currentTimestamp, 
           congestionMode ? "ACTIVE" : "OFF",
           emergencyMode ? "ACTIVE" : "OFF");
    printf("Landing Queue: %d | Takeoff Queue: %d\n",
           landingQueue.size, takeoffQueue.size);
    
    printf("\nCompleted Flights (sorted by ID):\n");
    if (flightArchive == NULL) {
        printf("  None yet.\n");
    } else {
        inorderTraversal(flightArchive);
    }
}