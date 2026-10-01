#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>
#include <time.h>
#include "common.h"

typedef enum {NEW, READY, RUNNING, FINISHED} status_t;

typedef struct
{
    uint8_t PID;
    char PName;
    uint32_t arrival;
    uint8_t priority;
    uint8_t burst;
    uint8_t remaining;
    int32_t start;
    uint32_t finish;
    uint32_t wait;
    status_t status;
} PCB_t;

typedef struct node_s
{
    void * data;
    size_t dataSize;
    struct node_s * next;
} node_t;

typedef struct
{
    node_t * first;
    node_t * last;
} queue_t;

typedef node_t* list_t;

// Queue
void createQueue (queue_t * q);
int addToQueue (queue_t * q, const void * data, size_t dataSize);
int removeFromQueue (queue_t * q, void * dst, size_t sizeDst);
void clearQueue (queue_t * q);
int isQueueEmpty(const queue_t * q);

// List
void createList (list_t * pl);
int appendToList (list_t * pl, const void * data, size_t dataSize);
void clearList (list_t * pl);
void mapList (list_t * pl, void (*f)(void*));

// Process
int createProcess (queue_t * q, uint8_t PID, char name, uint8_t arrival);
void freeProcess (PCB_t * p);

void printPCB (void * process);
void showStats (const list_t * l, uint8_t numProcess);

#endif // PROCESS_H_INCLUDED
