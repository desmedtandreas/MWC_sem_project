#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <omp.h>
#include <string.h>
#include <math.h>
#include "state.h"
#include "graph.h"
#include "instance.h"
#include "minimum_cut.h"
#include "queue_generator.h"

#define PARALLEL_THRESHOLD 10

// Computes the weight change when a vertex moves between partitions
int getWeightChange(int* partition, int idx, int** graph) {
    int weight = 0;
    if (partition[idx] == 1) {
        for (int i = 0; i < idx; i++) {
            if (partition[i] == 0) weight += graph[idx][i];  // for computing weight change when moving to X
        }
    }
    else if (partition[idx] == 0) {
        for (int i = 0; i < idx; i++) {
            if (partition[i] == 1) weight += graph[idx][i];  // for computing weight change when moving to Y
        }
    }
    return weight;  // return the computed weight
}

// Computes the lower bound only for vertices that are still unassigned (from idx to n-1).
int computeLowerBound(int idx, int n, int *partition, int **graph) {
    int lowerBound = 0;
    for (int i = idx; i < n; i++) {
        int costIfX = 0, costIfY = 0;
        // Only vertices [0, idx) are assigned.
        for (int j = 0; j < idx; j++) {
            if (partition[j] == 1)
                costIfX += graph[i][j];
            else if (partition[j] == 0)
                costIfY += graph[i][j];
        }
        lowerBound += (costIfX < costIfY ? costIfX : costIfY); // add the minimum possible cost
    }
    return lowerBound;
}

// Reccursive function to find the minimum cut using a branch and bound DFS approach.
void bb_dfs(int n, int a, int **graph, State state, int* bestSolution) {
    // If all vertices have been assigned, update the best solution if needed.

    if (state.depth == n) {
        if (state.weight < *bestSolution) {
            #pragma omp critical
            {
                if (state.weight < *bestSolution) {
                    *bestSolution = state.weight;
                }
            }
        }
        return;
    }

    // Branch where vertex at depth is assigned to subset X
    state.partition[state.depth] = 0;
    int newWeightX = state.weight + getWeightChange(state.partition, state.depth, graph);
    State newStateX = newState(n, state.partition, state.depth + 1, state.cX + 1, state.cY, newWeightX);
    
    if (newStateX.cX <= n - a) { // Ensure there is still room in subset X
        if (newWeightX < *bestSolution) { // Prune if current weight is worse than best weight
            int lowerBound = newWeightX + computeLowerBound(newStateX.depth, n, newStateX.partition, graph);
            if (lowerBound < *bestSolution) { // Prune if lower bound is worse than best weight
                if (state.depth < PARALLEL_THRESHOLD) {
                    #pragma omp task shared(bestSolution) firstprivate(newStateX)
                    {
                        bb_dfs(n, a, graph, newStateX, bestSolution);
                    }
                } 
                else {
                    bb_dfs(n, a, graph, newStateX, bestSolution);
                }
            }
        }
    }

    // Branch where vertex at depth is assigned to subset Y
    state.partition[state.depth] = 1;
    int newWeightY = state.weight + getWeightChange(state.partition, state.depth, graph);
    State newStateY = newState(n, state.partition, state.depth + 1, state.cX, state.cY + 1, newWeightY);

    if (newStateY.cY <= a) { // Ensure there is still room in subset Y
        if (newWeightY < *bestSolution) { // Prune if current weight is worse than best weight
            int lowerBound = newWeightY + computeLowerBound(newStateY.depth, n, newStateY.partition, graph);
            if (lowerBound < *bestSolution) { // Prune if lower bound is worse than best weight
                if (state.depth < PARALLEL_THRESHOLD) {
                    #pragma omp task shared(bestSolution) firstprivate(newStateY)
                    {
                        bb_dfs(n, a, graph, newStateY, bestSolution);
                    }
                } 
                else {
                    bb_dfs(n, a, graph, newStateY, bestSolution);
                }
            }   
        }
    }
    #pragma omp taskwait
}


// Function for finding the minimum cut of a graph
int findMinimumCut(Instance *instance, State state, int bestSolution, int numThreads) {
    int n = instance->n;
    int a = instance->a;
    int **graph = instance->graph;

    #pragma omp parallel num_threads(numThreads)
    {
        #pragma omp single
        {
            bb_dfs(n, a, graph, state, &bestSolution);
        }
    }

    return bestSolution;
}