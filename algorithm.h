#ifndef ALGORITHM_H_INCLUDED
#define ALGORITHM_H_INCLUDED

#include "process.h"

#define PREEMPTIVE 1
#define NON_PREEMPTIVE 0

typedef enum {ALG_FCFS = 1, ALG_ROUNDROBIN = 2, ALG_SPN = 3, ALG_PSPN = 4, ALG_HPRN = 5} algorith_options;

int selectAlgorithm ();
int runSimulation (uint8_t numProcess, int preemptive, int (*dispatcher)(list_t*, void*, size_t));

int roundRobin (list_t * pl, void * dst, size_t dstSize);
int shortestProcessNext (list_t * pl, void * dst, size_t dstSize);
int highestPenaltyRatio (uint8_t numProcess);

#endif // ALGORITHM_H_INCLUDED
