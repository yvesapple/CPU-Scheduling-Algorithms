#include <stdio.h>
#include <string.h>
#include "process.h"

int createProcess (list_t * q, uint8_t PID, char name, uint8_t arrival)
{
    PCB_t newP;

    newP.PID = PID;
    newP.PName = name;
    newP.arrival = arrival;
    newP.priority = rand() % MAX_PRIORITY;
    newP.burst = 1 + rand() % MAX_QUANTUM;
    newP.remaining = newP.burst;
    newP.start = -1;
    newP.wait = 0;
    newP.penalty = 1;

    printf("Process %c arrived with Priority: %d PID: %d Burst: %d\n", name, newP.priority, PID, newP.burst);
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
           p->PName, p->arrival, p->priority, p->burst, p->start, p->finish, p->wait);
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

    if(p1->burst == p2->arrival)
    {
        return p1->arrival - p2->arrival;
    }

    return p1->burst - p2->burst;
}

void recalculatePenalty (void * process, const void * simTime)
{
    PCB_t * p = (PCB_t*)process;
    const uint8_t * time = (const uint8_t*) simTime;

    p->wait += *time - (p->burst - p->remaining);
    p->penalty = p->wait / *time;
}

int cmpPenalty (const void * s1, const void * s2)
{
    const PCB_t * p1 = (const PCB_t*)s1;
    const PCB_t * p2 = (const PCB_t*)s2;

    if(p2->penalty == p1->penalty)
    {
        if(p1->priority == p2->priority)
        {
            return p1->arrival - p2->arrival;
        }

        return p1->priority - p2->priority;
    }

    return p2->penalty - p1->penalty;
}
