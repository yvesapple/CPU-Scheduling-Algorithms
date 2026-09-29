#include <stdio.h>
#include <string.h>
#include "process.h"

void createQueue (queue_t * q)
{
    q->first = NULL;
    q->last = NULL;
}

int createProcess (queue_t * q, uint8_t PID, char name, uint8_t arrival)
{
    PCB_t newP;

    newP.PID = PID;
    newP.PName = name;
    newP.arrival = arrival;
    newP.priority = rand() % MAX_PRIORITY;
    newP.burst = rand() % MAX_QUANTUM;
    newP.remaining = newP.burst;

    printf("Process %c arrived with priority: %d and PID: %d\n", name, newP.priority, PID);
    addToQueue(q, &newP, sizeof(PCB_t));

    return 1;
}

int addToQueue (queue_t * q, const void * data, size_t dataSize)
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
        fprintf(stderr, "Out of memory\n");
        return 0;
    }

    memcpy(newNode->data, data, dataSize);
    newNode->dataSize = dataSize;
    newNode->next = NULL;

    if(!q->first)
    {
        q->first = newNode;
    }
    else
    {
        q->last->next = newNode;
    }

    q->last = newNode;

    return 1;
}

void freeProcess (PCB_t * p)
{
    free(p);
}

void clearQueue (queue_t * q)
{
    while(q->first)
    {
        node_t * aux = q->first;
        q->first = aux->next;
        free(aux->data);
        free(aux);
    }

    q->last = NULL;
}

int removeFromQueue (queue_t * q, void * dst, size_t sizeDst)
{
    if(!q->first)
    {
        return 0;
    }
    node_t * aux = q->first;

    q->first = q->first->next;

    memcpy(dst, aux->data, MIN(sizeDst, aux->dataSize));
    free(aux->data);
    free(aux);

    return 1;
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

void printPCB (void * process)
{
    PCB_t * p = (PCB_t*)process;
    printf("   %c\t|%5hu \t  |%4hhu\t     |%4hhu   |%4hu   |%4hu    |%4hu\n",
           p->PName, p->arrival, p->priority, p->burst, p->start, p->finish, p->wait);
}

void showStats (const list_t * l, uint8_t numProcess)
{
    puts("");
    printf("Process | Arrival | Priority | Burst | Start | Finish | Wait\n");
    mapList((list_t*)l, printPCB);
}
