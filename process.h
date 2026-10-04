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

typedef node_t* list_t;

// List
void createList (list_t * pl);
int appendToList (list_t * pl, const void * data, size_t dataSize);
int getFirstList (list_t * pl, void * dst, size_t dstSize);
int isListEmpty (const list_t * pl);
void clearList (list_t * pl);
void mapList (list_t * pl, void (*f)(void*));
int getMinList (list_t * pl, void * dst, size_t dstSize, int(*cmp)(const void*, const void*));

// Process
int createProcess (list_t * q, uint8_t PID, char name, uint8_t arrival);
void freeProcess (PCB_t * p);

void printPCB (void * process);
void showStats (const list_t * l, uint8_t numProcess);

int cmpBurst (const void * s1, const void * s2);

#endif // PROCESS_H_INCLUDED
