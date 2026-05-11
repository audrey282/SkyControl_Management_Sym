#ifndef QUEUE_H
#define QUEUE_H

#include "flight.h"

typedef struct QueueNode {
    Flight* flight;
    struct QueueNode* next;
} QueueNode;

typedef struct Queue {
    QueueNode* front;
    QueueNode* rear;
    int size;
} Queue;

// Queue operations
void initQueue(Queue* q);
void enqueue(Queue* q, Flight* flight);
void enqueuePriority(Queue* q, Flight* flight);  // Priority-based insertion
Flight* dequeue(Queue* q);
Flight* peekQueue(Queue* q);
int isQueueEmpty(Queue* q);
void displayQueue(Queue* q, char* name);
void freeQueue(Queue* q);

#endif