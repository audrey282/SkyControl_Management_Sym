#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "simulator.h"
#include "flight.h"
#include "queue.h"
#include "runway.h"
#include "bst.h"
#include "stack.h"

#define MAX_FLIGHT_ID 20

// Function prototypes for menu operations
void addNewFlight();
void generateRandomFlight();
void displayRunwayStatus();
void displayQueues();
void searchCompletedFlight();
void clearScreen();

void addNewFlight() {
    char id[MAX_FLIGHT_ID];
    int fuel, typeChoice, emergency;
    
    printf("\n=== ADD NEW FLIGHT ===\n");
    printf("Flight ID: ");
    scanf("%s", id);
    printf("Fuel Level (0-100): ");
    scanf("%d", &fuel);
    printf("Type (1=Landing, 2=Takeoff): ");
    scanf("%d", &typeChoice);
    printf("Emergency? (1=Yes, 0=No): ");
    scanf("%d", &emergency);
    
    if (fuel < 0 || fuel > 100) {
        printf("Invalid fuel! Setting to 50.\n");
        fuel = 50;
    }
    
    char type[10];
    if (typeChoice == 1) {
        strcpy(type, "Landing");
    } else {
        strcpy(type, "Takeoff");
    }
    
    Flight* newFlight = createFlight(id, fuel, type, emergency);
    
    if (typeChoice == 1) {
        enqueuePriority(&landingQueue, newFlight);
        printf("✅ Flight %s added to LANDING queue\n", id);
    } else {
        enqueuePriority(&takeoffQueue, newFlight);
        printf("✅ Flight %s added to TAKEOFF queue\n", id);
    }
    
    if (emergency) {
        printf("⚠️  MAYDAY declared for %s!\n", id);
    }
}

void generateRandomFlight() {
    static int flightNum = 100;
    char id[MAX_FLIGHT_ID];
    int type = rand() % 2;
    int emergency = (rand() % 10 == 0);
    int fuel = emergency ? (rand() % 30 + 10) : (rand() % 80 + 20);
    
    sprintf(id, "FL%03d", flightNum++);
    
    printf("\n=== RANDOM FLIGHT GENERATED ===\n");
    printf("  %s | %s | Fuel: %d%% | Emergency: %s\n",
           id, (type == 0) ? "Landing" : "Takeoff", fuel, emergency ? "YES" : "No");
    
    char typeStr[10];
    if (type == 0) {
        strcpy(typeStr, "Landing");
    } else {
        strcpy(typeStr, "Takeoff");
    }
    
    Flight* newFlight = createFlight(id, fuel, typeStr, emergency);
    
    if (type == 0) {
        enqueuePriority(&landingQueue, newFlight);
    } else {
        enqueuePriority(&takeoffQueue, newFlight);
    }
}

void displayRunwayStatus() {
    printf("\n=== RUNWAY STATUS ===\n");
    displayRunway(&runwayAlpha);
    displayRunway(&runwayBravo);
}

void displayQueues() {
    displayQueue(&landingQueue, "Landing");
    displayQueue(&takeoffQueue, "Takeoff");
}

void searchCompletedFlight() {
    char searchID[MAX_FLIGHT_ID];
    printf("\n=== SEARCH COMPLETED FLIGHT ===\n");
    printf("Enter Flight ID: ");
    scanf("%s", searchID);
    
    BSTNode* result = searchBST(flightArchive, searchID);
    if (result != NULL) {
        printf("\n✅ FLIGHT FOUND!\n  ");
        printFlight(result->flight);
        printf(" | Completed at: t=%d\n", result->flight->timestamp);
    } else {
        printf("\n❌ Flight %s not found in archive.\n", searchID);
    }
}

void displayMenu() {
    printf("\n========================================\n");
    printf("✈️  SKYCONTROL - RUNWAY MANAGEMENT SYSTEM\n");
    printf("========================================\n");
    printf("1. Add New Flight\n");
    printf("2. Process Next Time Step\n");
    printf("3. Display Queues\n");
    printf("4. Display Runway Status\n");
    printf("5. Undo Last Assignment\n");
    printf("6. Declare Emergency (Mayday)\n");
    printf("7. Search Completed Flight\n");
    printf("8. Display Statistics & Archive\n");
    printf("9. Generate Random Flight\n");
    printf("0. Exit System\n");
    printf("----------------------------------------\n");
    printf("Choice: ");
}

void clearScreen() {
    printf("\n\n\n\n");
}

int main() {
    srand(time(NULL));
    initSimulation();
    
    // Add some initial flights for demonstration
    Flight* f1 = createFlight("AAL101", 75, "Landing", 0);
    Flight* f2 = createFlight("UAL202", 60, "Takeoff", 0);
    Flight* f3 = createFlight("DAL303", 45, "Landing", 0);
    enqueuePriority(&landingQueue, f1);
    enqueuePriority(&takeoffQueue, f2);
    enqueuePriority(&landingQueue, f3);
    
    printf("\n✈️  SKYCONTROL AVIATION SYSTEMS INITIALIZED ✈️\n");
    printf("Welcome to the Multi-Runway Management Simulator!\n");
    printf("------------------------------------------------\n");
    
    int choice;
    do {
        displayMenu();
        scanf("%d", &choice);
        
        switch(choice) {
            case 1: addNewFlight(); break;
            case 2: processTimeStep(); break;
            case 3: displayQueues(); break;
            case 4: displayRunwayStatus(); break;
            case 5: undoLastAssignment(); break;
            case 6: processEmergency(); break;
            case 7: searchCompletedFlight(); break;
            case 8: displayStatistics(); break;
            case 9: generateRandomFlight(); break;
            case 0:
                printf("\nShutting down SkyControl System...\n");
                printf("Thank you for using the Runway Management Simulator!\n");
                break;
            default:
                printf("\nInvalid choice! Please try again.\n");
        }
        
        if (choice != 0) {
            printf("\nPress Enter to continue...");
            getchar();
            getchar();
            clearScreen();
        }
    } while (choice != 0);
    
    // Cleanup
    freeQueue(&landingQueue);
    freeQueue(&takeoffQueue);
    freeStack(&historyStack);
    freeBST(flightArchive);
    
    return 0;
}