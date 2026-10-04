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

    printf("Process %c arrived with Priority: %d PID: %d Burst: %d\n", name, newP.priority, PID, newP.burst);
    appendToList(q, &newP, sizeof(PCB_t));

    return 1;
}

void freeProcess (PCB_t * p)
{
    free(p);
}

void createList (list_t * pl)
{
    *pl = NULL;
}

int appendToList (list_t * pl, const void * data, size_t dataSize)
{
    node_t * newNode = malloc(sizeof(node_t));
    if(!newNode)
    {
        fprintf(stderr, "Out of memory\n");
        return 0;
    }

    newNode->data = malloc(dataSize);
    if(!newNode->data)
    {
        free(newNode);
        return 0;
    }

    memcpy(newNode->data, data, dataSize);
    newNode->dataSize = dataSize;
    newNode->next = NULL;

    while(*pl)
    {
        pl = &(*pl)->next;
    }

    *pl = newNode;

    return 1;
}

int getFirstList (list_t * pl, void * dst, size_t dstSize)
{
    if(!*pl)
    {
        return 0;
    }

    memcpy(dst, (*pl)->data, MIN((*pl)->dataSize, dstSize));

    node_t * aux = (*pl);
    *pl = (*pl)->next;

    free(aux->data);
    free(aux);

    return 1;
}

int isListEmpty (const list_t * pl)
{
    return *pl == NULL;
}

void clearList (list_t * pl)
{
    while(*pl)
    {
        node_t * aux = *pl;
        *pl = aux->next;

        free(aux->data);
        free(aux);
    }
}

void mapList (list_t * pl, void (*f)(void*))
{
    while(*pl)
    {
        f((*pl)->data);

        pl = &(*pl)->next;
    }
}

int getMinList (list_t * pl, void * dst, size_t dstSize, int(*cmp)(const void*, const void*))
{
    if(!*pl)
    {
        return 0;
    }

    node_t * prev = NULL;
    node_t * curr = *pl;
    node_t * minPrev = NULL;
    node_t * min = *pl;

    while(curr)
    {
        if(cmp(curr->data, min->data) < 0)
        {
            min = curr;
            minPrev = prev;
        }

        prev = curr;
        curr = curr->next;
    }

    memcpy(dst, min->data, MIN(min->dataSize, dstSize));

    if(!minPrev)
    {
        *pl = min->next;    // first is the min
    }
    else
    {
        minPrev->next = min->next;
    }

    free(min->data);
    free(min);

    return 1;
}

void printPCB (void * process)
{
    PCB_t * p = (PCB_t*)process;
    printf("   %c\t|%5hu \t  |%4hhu\t     |%4hhu   |%4d   |%4hu    |%4hu\n",
           p->PName, p->arrival, p->priority, p->burst, p->start, p->finish, p->wait);
}

void showStats (const list_t * l, uint8_t numProcess)
{
    puts("");
    printf("Process | Arrival | Priority | Burst | Start | Finish | Wait\n");
    mapList((list_t*)l, printPCB);
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
