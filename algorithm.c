#include <stdio.h>
#include <string.h>
#include "algorithm.h"

int processAlgorithms ()
{
    uint8_t option = 0;
    uint8_t numProcess = 0;
    int result = 0;

    showMenu();

    option = getInt(1, NUM_OPTIONS);

    printf(BLUE "==> Number of processes (1 - 27): " RESET);
    numProcess = getInt(MIN_PROCESSES, MAX_PROCESSES);

    switch(option)
    {
    case A_FCFS:
        result = simulation(numProcess, NON_PREEMPTIVE, getFirstList);
        break;
    case A_ROUNDROBIN:
        result = simulation(numProcess, PREEMPTIVE, roundRobin);
        break;
    case A_SPN:
        result = simulation(numProcess, NON_PREEMPTIVE, shortestProcessNext);
        break;
    case A_PSPN:    // Preemptive Shortess Process Next
        result = simulation(numProcess, PREEMPTIVE, shortestProcessNext);
        break;
    case A_HPRN:     // Highest Penalty Ratio Next
        result = highestPenaltyRatio(numProcess);
    }

    return result;
}

int simulation (uint8_t numProcess, int preemptive, int (*dispatcher)(list_t*, void*, size_t))
{
    list_t readyQueue;
    list_t finishedList;

    createList(&readyQueue);
    createList(&finishedList);

    uint8_t i = 0;
    uint32_t simTime = 0;
    uint8_t quantum = 0;

    if(preemptive)
    {
        printf(BLUE "==> Quantum time (1 - 256): " RESET);
        quantum = getInt(1, MAX_QUANTUM);
    }

    uint8_t quantumLeft = quantum;
    char processName = 'A';

    if(!createProcess(&readyQueue, i, processName, simTime))
    {
        return E_NOMEM;
    }

    i++;
    processName++;

    PCB_t running;
    PCB_t preempted;
    getFirstList(&readyQueue, &running, sizeof(PCB_t));
    running.start = simTime;
    running.status = RUNNING;
    printf("Running process %c\n", running.PName);

    while(running.status != FINISHED)
    {
        simTime++;

        if(preemptive && !isListEmpty(&readyQueue) && quantumLeft == 0)
        {
            running.status = READY;
            printf("Preempting process %c (%hhu remaining)\n", running.PName, running.remaining);
            preempted = running;

            dispatcher(&readyQueue, &running, sizeof(PCB_t));
            printf("Now running process %c\n", running.PName);

            appendToList(&readyQueue, &preempted, sizeof(PCB_t));

            if(running.start == -1)
            {
                running.start = simTime;
            }

            quantumLeft = quantum;
        }

        if(i < numProcess && ((rand() % 5) == 0))
        {
            if(!createProcess(&readyQueue, i, processName, simTime))
            {
                clearList(&readyQueue);
                clearList(&finishedList);
                return E_NOMEM;
            }

            i++;
            processName++;
        }

        else if(running.remaining)
        {
            running.remaining--;
            quantumLeft--;
        }

        else if(!running.remaining)
        {
            running.status = FINISHED;
            running.finish = simTime;
            running.wait = running.finish - running.arrival - running.burst;
            appendToList(&finishedList, &running, sizeof(PCB_t));
            printf("Process %c finished\n", running.PName);

            quantumLeft = quantum;

            if(dispatcher(&readyQueue, &running, sizeof(PCB_t)))
            {
                printf("Now running process %c\n", running.PName);

                if(running.start == -1)
                {
                    running.start = simTime;
                }

                if(preemptive && isListEmpty(&readyQueue))
                {
                    printf("Process %c is the only available, skipping the quantum\n", running.PName);
                }
            }
        }
    }

    clearList(&readyQueue);
    showStats(&finishedList, numProcess);
    clearList(&finishedList);

    return E_SUCCESS;
}

int roundRobin (list_t * pl, void * dst, size_t dstSize)
{
    return getFirstList(pl, dst, dstSize);
}

int shortestProcessNext (list_t * pl, void * dst, size_t dstSize)
{
    return getMinList(pl, dst, dstSize, cmpBurst);
}

int highestPenaltyRatio (uint8_t numProcess)
{
    list_t readyQueue;
    list_t finishedList;

    createList(&readyQueue);
    createList(&finishedList);

    uint8_t i = 0;
    uint32_t simTime = 0;

    char processName = 'A';

    if(!createProcess(&readyQueue, i, processName, simTime))
    {
        return E_NOMEM;
    }

    i++;
    processName++;

    PCB_t running;
    getFirstList(&readyQueue, &running, sizeof(PCB_t));
    running.start = simTime;
    running.status = RUNNING;
    printf("Running process %c\n", running.PName);

    while(running.status != FINISHED)
    {
        simTime++;

        if(i < numProcess && ((rand() % 5) == 0))
        {
            if(!createProcess(&readyQueue, i, processName, simTime))
            {
                clearList(&readyQueue);
                clearList(&finishedList);
                return E_NOMEM;
            }

            i++;
            processName++;
        }

        else if(running.remaining)
        {
            running.remaining--;
        }

        else
        {
            running.status = FINISHED;
            running.finish = simTime;
            appendToList(&finishedList, &running, sizeof(PCB_t));
            printf("Process %c finished\n", running.PName);

            mapList(&readyQueue, &simTime, recalculatePenalty);

            if(getMinList(&readyQueue, &running, sizeof(PCB_t), cmpPenalty))
            {
                printf("Now running process %c\n", running.PName);
                if(running.start == -1)
                {
                    running.start = simTime;
                }
            }
        }
    }

    clearList(&readyQueue);
    showStats(&finishedList, numProcess);
    clearList(&finishedList);

    return E_SUCCESS;
}
