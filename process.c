#include <stdio.h>
#include <string.h>
#include "process.h"

int createProcess (list_t * q, uint8_t pid, char name, uint8_t arrival)
{
    PCB_t newP;

    newP.pid = pid;
    newP.name = name;
    newP.arrivalTime = arrival;
    newP.priority = rand() % MAX_PRIORITY;
    newP.burstTime = 1 + rand() % MAX_QUANTUM;
    newP.remainingTime = newP.burstTime;
    newP.startTime = -1;
    newP.waitTime = 0;
    newP.penaltyRatio = 1;

    printf("Process %c arrived with Priority: %d PID: %d Burst: %d\n", name, newP.priority, pid, newP.burstTime);
    appendToList(q, &newP, sizeof(PCB_t));

    return 1;
}

void freeProcess (PCB_t * p)
{
    free(p);
}

void printPCB (void * process, const void * param)
{
    PCB_t * p = (PCB_t*)process;
    printf("   %c\t|%5hu \t  |%4hhu\t     |%4hhu   |%4d   |%4hu    |%4hu\n",
           p->name, p->arrivalTime, p->priority, p->burstTime, p->startTime, p->finishTime, p->waitTime);
}

void showStats (const list_t * l, uint8_t numProcess)
{
    puts("");
    printf("Process | Arrival | Priority | Burst | Start | Finish | Wait\n");
    mapList((list_t*)l, NULL, printPCB);
}

int cmpBurst (const void * s1, const void * s2)
{
    const PCB_t * p1 = (const PCB_t*)s1;
    const PCB_t * p2 = (const PCB_t*)s2;

    if(p1->burstTime == p2->burstTime)
    {
        return p1->arrivalTime - p2->arrivalTime;
    }

    return p1->burstTime - p2->burstTime;
}

void recalculatePenalty (void * process, const void * simTime)
{
    PCB_t * p = (PCB_t*)process;
    const uint8_t * time = (const uint8_t*) simTime;

    p->waitTime += *time - (p->burstTime - p->remainingTime);
    p->penaltyRatio = (double)p->waitTime / *time;
}

int cmpPenalty (const void * s1, const void * s2)
{
    const PCB_t * p1 = (const PCB_t*)s1;
    const PCB_t * p2 = (const PCB_t*)s2;

    if(p2->penaltyRatio == p1->penaltyRatio)
    {
        if(p1->priority == p2->priority)
        {
            return p1->arrivalTime - p2->arrivalTime;
        }

        return p1->priority - p2->priority;
    }

    return p2->penaltyRatio - p1->penaltyRatio;
}
