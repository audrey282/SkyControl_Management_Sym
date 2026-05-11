#include <stdio.h>
#include <stdlib.h>
#include "queue.h"
#include "flight.h"

void initQueue(Queue* q) {
    q->front = q->rear = NULL;
    q->size = 0;
}

// Regular FIFO enqueue
void enqueue(Queue* q, Flight* flight) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->flight = flight;
    newNode->next = NULL;
    
    if (q->rear == NULL) {
        q->front = q->rear = newNode;
    } else {
        q->rear->next = newNode;
        q->rear = newNode;
    }
    q->size++;
}

// Priority enqueue - inserts in order of priority score
void enqueuePriority(Queue* q, Flight* flight) {
    QueueNode* newNode = (QueueNode*)malloc(sizeof(QueueNode));
    newNode->flight = flight;
    newNode->next = NULL;
    
    int newScore = getPriorityScore(flight);
    
    // Case 1: Empty queue or new flight has highest priority
    if (q->front == NULL || newScore < getPriorityScore(q->front->flight)) {
        newNode->next = q->front;
        q->front = newNode;
        if (q->rear == NULL) {
            q->rear = newNode;
        }
    } 
    // Case 2: Insert in middle or end
    else {
        QueueNode* current = q->front;
        while (current->next != NULL && 
               getPriorityScore(current->next->flight) <= newScore) {
            current = current->next;
        }
        newNode->next = current->next;
        current->next = newNode;
        if (newNode->next == NULL) {
            q->rear = newNode;
        }
    }
    q->size++;
}

Flight* dequeue(Queue* q) {
    if (isQueueEmpty(q)) return NULL;
    
    QueueNode* temp = q->front;
    Flight* flight = temp->flight;
    q->front = q->front->next;
    
    if (q->front == NULL) {
        q->rear = NULL;
    }
    
    free(temp);
    q->size--;
    return flight;
}

Flight* peekQueue(Queue* q) {
    if (isQueueEmpty(q)) return NULL;
    return q->front->flight;
}

int isQueueEmpty(Queue* q) {
    return q->front == NULL;
}

void displayQueue(Queue* q, char* name) {
    printf("\n--- %s Queue (Size: %d) ---\n", name, q->size);
    if (isQueueEmpty(q)) {
        printf("  Empty\n");
        return;
    }
    
    QueueNode* current = q->front;
    int position = 1;
    while (current != NULL) {
        printf("  %d. ", position++);
        printFlight(current->flight);
        printf("\n");
        current = current->next;
    }
}

void freeQueue(Queue* q) {
    while (!isQueueEmpty(q)) {
        Flight* f = dequeue(q);
        freeFlight(f);
    }
}