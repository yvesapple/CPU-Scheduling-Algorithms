#include <stdio.h>
#include <string.h>
#include "algorithm.h"

int selectAlgorithm ()
{
    uint8_t option = 0;
    uint8_t numProcess = 0;
    int result = 0;

    showMenu();

    option = getInt(1, NUM_ALGORITHMS);

    printf(BLUE "==> Number of processes (1 - 27): " RESET);
    numProcess = getInt(MIN_PROCESSES, MAX_PROCESSES);

    switch(option)
    {
    case ALG_FCFS:
        printf(BLUE "\nFirst Come, First Serve\n" RESET);
        result = runSimulation(numProcess, NON_PREEMPTIVE, popFrontList);
        break;
    case ALG_ROUNDROBIN:
        printf(BLUE "\nRound Robin\n" RESET);
        result = runSimulation(numProcess, PREEMPTIVE, roundRobin);
        break;
    case ALG_SPN:
        printf(BLUE "\nShortest Process Next\n" RESET);
        result = runSimulation(numProcess, NON_PREEMPTIVE, shortestProcessNext);
        break;
    case ALG_PSPN:
        printf(BLUE "\nPreemptive Shortess Process Next\n" RESET);
        result = runSimulation(numProcess, PREEMPTIVE, shortestProcessNext);
        break;
    case ALG_HPRN:
        printf(BLUE "\nHighest Penalty Ratio Next\n" RESET);
        result = highestPenaltyRatio(numProcess);
        break;
    }

    return result;
}

int runSimulation (uint8_t numProcess, int preemptive, int (*dispatcher)(list_t*, void*, size_t))
{
    list_t readyQueue;
    list_t finishedList;

    initList(&readyQueue);
    initList(&finishedList);

    uint8_t i = 0;
    uint32_t simTime = 0;
    uint8_t quantum = 0;

    if(preemptive)
    {
        printf(BLUE "==> Quantum time (1 - 256): " RESET);
        quantum = getInt(1, MAX_QUANTUM);
    }

    uint8_t remainingQuantum = quantum;
    char processName = 'A';

    if(!createProcess(&readyQueue, i, processName, simTime))
    {
        return E_NOMEM;
    }

    i++;
    processName++;

    PCB_t running;
    PCB_t preempted;
    popFrontList(&readyQueue, &running, sizeof(PCB_t));
    running.startTime = simTime;
    running.status = RUNNING;
    printf("Running process %c\n", running.name);

    while(running.status != FINISHED)
    {
        simTime++;

        if(preemptive && !isEmptyList(&readyQueue) && remainingQuantum == 0)
        {
            running.status = READY;
            printf("Preempting process %c (%hhu remaining)\n", running.name, running.remainingTime);
            preempted = running;

            dispatcher(&readyQueue, &running, sizeof(PCB_t));
            printf("Now running process %c\n", running.name);

            appendToList(&readyQueue, &preempted, sizeof(PCB_t));

            if(running.startTime == -1)
            {
                running.startTime = simTime;
            }

            remainingQuantum = quantum;
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

        else if(running.remainingTime)
        {
            running.remainingTime--;
            remainingQuantum--;
        }

        else if(!running.remainingTime)
        {
            running.status = FINISHED;
            running.finishTime = simTime;
            running.waitTime = running.finishTime - running.arrivalTime - running.burstTime;
            appendToList(&finishedList, &running, sizeof(PCB_t));
            printf("Process %c finished\n", running.name);

            remainingQuantum = quantum;

            if(dispatcher(&readyQueue, &running, sizeof(PCB_t)))
            {
                printf("Now running process %c\n", running.name);

                if(running.startTime == -1)
                {
                    running.startTime = simTime;
                }

                if(preemptive && isEmptyList(&readyQueue))
                {
                    printf("Process %c is the only available, skipping the quantum\n", running.name);
                }
            }
        }
    }

    clearList(&readyQueue);
    showStats(&finishedList);
    clearList(&finishedList);

    return E_SUCCESS;
}

int roundRobin (list_t * pl, void * dst, size_t dstSize)
{
    return popFrontList(pl, dst, dstSize);
}

int shortestProcessNext (list_t * pl, void * dst, size_t dstSize)
{
    return removeMinList(pl, dst, dstSize, cmpBurst);
}

int highestPenaltyRatio (uint8_t numProcess)
{
    list_t readyQueue;
    list_t finishedList;

    initList(&readyQueue);
    initList(&finishedList);

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
    popFrontList(&readyQueue, &running, sizeof(PCB_t));
    running.startTime = simTime;
    running.status = RUNNING;
    printf("Running process %c\n", running.name);

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

        else if(running.remainingTime)
        {
            running.remainingTime--;
        }

        else
        {
            running.status = FINISHED;
            running.finishTime = simTime;
            appendToList(&finishedList, &running, sizeof(PCB_t));
            printf("Process %c finished\n", running.name);

            mapList(&readyQueue, &simTime, recalculatePenalty);

            if(removeMinList(&readyQueue, &running, sizeof(PCB_t), cmpPenalty))
            {
                printf("Now running process %c\n", running.name);
                if(running.startTime == -1)
                {
                    running.startTime = simTime;
                }
            }
        }
    }

    clearList(&readyQueue);
    showStats(&finishedList);
    clearList(&finishedList);

    return E_SUCCESS;
}
