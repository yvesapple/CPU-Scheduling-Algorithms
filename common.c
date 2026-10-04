#include <stdio.h>
#include "common.h"

void showMenu ()
{
    printf(BOLD BLUE "=== Scheduling Algorithms ===\n" RESET);
    printf(BLUE "\t1. FCFS\n");
    printf("\t2. Round Robin\n");
    printf("\t3. SPN\n" RESET);
    puts("");

    printf(BLUE "==> ");
    printf("Select an option (1 - 9): " RESET);
}

int getInt (int li, int ls)
{
    int value;

    scanf("%d", &value);
    while(value < li || value > ls)
    {
        printf(BLUE "Invalid value\n");
        printf("==> " RESET);
        scanf("%d", &value);
    }

    puts("");
    return value;
}
