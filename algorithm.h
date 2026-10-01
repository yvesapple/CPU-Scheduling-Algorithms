#ifndef ALGORITHM_H_INCLUDED
#define ALGORITHM_H_INCLUDED

#include "process.h"

typedef enum {A_FCFS = 1, A_ROUNDROBIN = 2} algorith_options;

int processAlgorithms ();

int FCFS (uint8_t numProcess);
int roundRobin (uint8_t numProcess);

#endif // ALGORITHM_H_INCLUDED
