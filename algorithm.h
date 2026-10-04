#ifndef ALGORITHM_H_INCLUDED
#define ALGORITHM_H_INCLUDED

#include "process.h"

#define PREEMPTIVE 1
#define NON_PREEMPTIVE 0

typedef enum {A_FCFS = 1, A_ROUNDROBIN = 2, A_SPN = 3} algorith_options;

int processAlgorithms ();
int simulation (uint8_t numProcess, int preemptive, int (*dispatcher)(list_t*, void*, size_t));

int roundRobin (list_t * pl, void * dst, size_t dstSize);
int shortestProcessNext (list_t * pl, void * dst, size_t dstSize);

#endif // ALGORITHM_H_INCLUDED
