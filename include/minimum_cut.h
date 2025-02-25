#ifndef MINIMUM_CUT_H
#define MINIMUM_CUT_H

#include <instance.h>

typedef struct Solution {
    int* partition;
    int minWeight;
    int recCalls;
    double time;
} Solution;

void printSolution(Solution solution, int n);

Solution findMinimumCut(Instance* instance);

#endif