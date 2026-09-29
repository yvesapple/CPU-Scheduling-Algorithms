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

    puts("");

    switch(option)
    {
        case A_FCFS: result = FCFS(numProcess); break;
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
    uint32_t qTime = 0;         // quantum time
    char processName = 'A';

    if(!createProcess(&readyQueue, i, processName, qTime))
    {
        return E_NOMEM;
    }

    i++;
    processName++;

    PCB_t running;
    removeFromQueue(&readyQueue, &running, sizeof(PCB_t));
    running.start = qTime;
    running.status = RUNNING;
    printf("Running process %c\n", running.PName);

    while(running.status != FINISHED)
    {
        qTime++;

        if(i < numProcess && ((rand() % 5) == 0))
        {
            if(!createProcess(&readyQueue, i, processName, qTime))
            {
                clearQueue(&readyQueue);
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
            running.finish = qTime;
            appendToList(&finishedList, &running, sizeof(PCB_t));
            printf("Process %c finished\n", running.PName);

            if(removeFromQueue(&readyQueue, &running, sizeof(PCB_t)))
            {
                printf("Running process %c\n", running.PName);
                running.start = qTime;
                running.wait = running.start - running.arrival;
            }
        }
    }

    clearQueue(&readyQueue);
    showStats(&finishedList, numProcess);
    clearList(&finishedList);

    return E_SUCCESS;
}
