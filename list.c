#include <stdio.h>
#include <string.h>
#include "common.h"
#include "list.h"

void initList (list_t * pl)
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

int popFrontList (list_t * pl, void * dst, size_t dstSize)
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

int isEmptyList (const list_t * pl)
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

void mapList (list_t * pl, const void * param, void (*f)(void*, const void*))
{
    while(*pl)
    {
        f((*pl)->data, param);

        pl = &(*pl)->next;
    }
}

int removeMinList (list_t * pl, void * dst, size_t dstSize, int(*cmp)(const void*, const void*))
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
