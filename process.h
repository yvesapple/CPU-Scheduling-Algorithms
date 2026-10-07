#ifndef PROCESS_H_INCLUDED
#define PROCESS_H_INCLUDED

#include <time.h>
#include "list.h"
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
    uint8_t penalty;
    status_t status;
} PCB_t;

int createProcess (list_t * q, uint8_t PID, char name, uint8_t arrival);
void freeProcess (PCB_t * p);

void recalculatePenalty (void * process, const void * simTime);

void printPCB (void * process, const void * param);
void showStats (const list_t * l, uint8_t numProcess);

int cmpBurst (const void * s1, const void * s2);
int cmpPenalty (const void * s1, const void * s2);

#endif // PROCESS_H_INCLUDED
