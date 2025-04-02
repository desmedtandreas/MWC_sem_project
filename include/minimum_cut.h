#ifndef MINIMUM_CUT_H
#define MINIMUM_CUT_H

#include <instance.h>

/**
 * @brief Find the Minimum Weighted Cut in a graph
 *
 * @param instance The given problem instance
 * @return Solution The found solution to the minimum weight cut problem
 */
int findMinimumCut(Instance *instance, State state, int bestSolution, int numThreads);

int getWeightChange(int* partition, int idx, int** graph);

#endif