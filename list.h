#ifndef LIST_H_INCLUDED
#define LIST_H_INCLUDED

#include <stdint.h>
#include <stdlib.h>

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
void mapList (list_t * pl, const void * param, void (*f)(void*, const void*));
int getMinList (list_t * pl, void * dst, size_t dstSize, int(*cmp)(const void*, const void*));

#endif // LIST_H_INCLUDED
