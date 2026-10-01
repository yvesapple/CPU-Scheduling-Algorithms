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
        result = FCFS(numProcess);
        break;
    case A_ROUNDROBIN:
        result = roundRobin(numProcess);
        break;
    }

    return result;
}

int FCFS (uint8_t numProcess)
{
    queue_t readyQueue;
    list_t finishedList;

    createQueue(&readyQueue);
    createList(&finishedList);

    uint8_t i = 0;
    uint32_t simTime = 0;         // current time
    char processName = 'A';

    if(!createProcess(&readyQueue, i, processName, simTime))
    {
        return E_NOMEM;
    }

    i++;
    processName++;

    PCB_t running;
    removeFromQueue(&readyQueue, &running, sizeof(PCB_t));
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
                clearQueue(&readyQueue);
                clearList(&finishedList);
                return E_NOMEM;
            }

            i++;
            processName++;
        }

        if(running.remaining)
        {
            running.remaining--;
        }

        if(!running.remaining)
        {
            running.status = FINISHED;
            running.finish = simTime;
            appendToList(&finishedList, &running, sizeof(PCB_t));
            printf("Process %c finished\n", running.PName);

            if(removeFromQueue(&readyQueue, &running, sizeof(PCB_t)))
            {
                printf("Running process %c\n", running.PName);
                running.start = simTime;
                running.wait = running.start - running.arrival;
            }
        }
    }

    clearQueue(&readyQueue);
    showStats(&finishedList, numProcess);
    clearList(&finishedList);

    return E_SUCCESS;
}

int roundRobin (uint8_t numProcess)
{
    queue_t readyQueue;
    list_t finishedList;

    createQueue(&readyQueue);
    createList(&finishedList);

    uint8_t i = 0;
    uint32_t simTime = 0;

    printf(BLUE "==> Quantum time (1 - 256): " RESET);
    uint8_t quantum = getInt(1, MAX_QUANTUM);

    uint8_t quantumLeft = quantum;
    char processName = 'A';

    if(!createProcess(&readyQueue, i, processName, simTime))
    {
        return E_NOMEM;
    }

    i++;
    processName++;

    PCB_t running;
    removeFromQueue(&readyQueue, &running, sizeof(PCB_t));
    running.start = simTime;
    running.status = RUNNING;
    printf("Running process %c\n", running.PName);

    while(running.status != FINISHED)
    {
        simTime++;

        if(!isQueueEmpty(&readyQueue) && quantumLeft == 0)
        {
            running.status = READY;
            printf("Preempting process %c (%hhu remaining)\n", running.PName, running.remaining);
            addToQueue(&readyQueue, &running, sizeof(PCB_t));

            removeFromQueue(&readyQueue, &running, sizeof(PCB_t));

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
                clearQueue(&readyQueue);
                clearList(&finishedList);
                return E_NOMEM;
            }

            i++;
            processName++;
        }

        if(running.remaining)
        {
            running.remaining--;
            quantumLeft--;
        }

        if(!running.remaining)
        {
            running.status = FINISHED;
            running.finish = simTime;
            running.wait = running.finish - running.arrival - running.burst;
            appendToList(&finishedList, &running, sizeof(PCB_t));
            printf("Process %c finished\n", running.PName);

            quantumLeft = quantum;

            if(removeFromQueue(&readyQueue, &running, sizeof(PCB_t)))
            {
                printf("Running process %c\n", running.PName);
                if(running.start == -1)
                {
                    running.start = simTime;
                }

                if(isQueueEmpty(&readyQueue))
                {
                    printf("Process %c is the only available, skipping the quantum\n", running.PName);
                }
            }
        }
    }

    clearQueue(&readyQueue);
    showStats(&finishedList, numProcess);
    clearList(&finishedList);

    return E_SUCCESS;
}
