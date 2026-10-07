#include <stdio.h>
#include <stdlib.h>
#include "common.h"

void showMenu ()
{
    printf(BOLD BLUE "=== Scheduling Algorithms ===\n" RESET);
    printf(BLUE "\t1. First Come First Serve\n");
    printf("\t2. Round Robin\n");
    printf("\t3. Shortest Process Next\n");
    printf("\t4. Preemptive Shortest Process Next\n");
    printf("\t5. Highest Penalty Ratio Next\n");
    printf(RESET "\n");

    printf(BLUE "==> ");
    printf("Select an option (1 - 9): " RESET);
}

int getInt (int li, int ls)
{
    int value;

    scanf("%d", &value);
    if(value < li || value > ls)
    {
        fprintf(stderr, "Error: Invalid argument\n");
        exit(E_ARG);
    }

    return value;
}
