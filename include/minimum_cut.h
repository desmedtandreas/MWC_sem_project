#ifndef MINIMUM_CUT_H
#define MINIMUM_CUT_H

#include <instance.h>

typedef struct Solution {
    int* partition;
    int minWeight;
    int recCalls;
} Solution;

Solution findMinimumCut(Instance* instance);

#endif