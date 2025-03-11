#ifndef MINIMUM_CUT_H
#define MINIMUM_CUT_H

#include <instance.h>

/**
 * @brief Struct to store the solution of the minimum cut problem
 */
typedef struct Solution {
    int* partition; ///< Array of size n that stores the partition of the vertices
    int minWeight;  ///< Minimum weight of the cut
    int recCalls;   ///< Number of recursive calls
    double time;    ///< Time taken to compute the cut
} Solution;

/**
 * @brief Print the solution of the minimum cut problem
 * 
 * @param solution Solution to be printed
 * @param n Number of vertices in the instance
 * @return void
 */
void printSolution(Solution solution, int n);

/**
 * @brief Find the Minimum Weighted Cut in a graph
 *
 * @param instance The given problem instance
 * @return Solution The found solution to the minimum weight cut problem
 */
Solution findMinimumCut(Instance* instance, int numThreads);

#endif