#include <stdio.h>
#include <stdlib.h>
#include <omp.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include "state.h"
#include "graph.h"
#include "instance.h"
#include "minimum_cut.h"

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
void bb_dfs(int n, int a, int **graph, State state, State* bestState, int *recCalls) {
    #pragma omp atomic
    (*recCalls)++;

    // If all vertices have been assigned, update the best solution if needed.
    if (state.depth == n) {
        #pragma omp critical
        {
            if (state.weight < bestState->weight) {
                *bestState = copyState(n, state);
            }
        }
        return;
    }

    // Branch where vertex at depth is assigned to subset X
    state.partition[state.depth] = 0;
    int newWeightX = state.weight + getWeightChange(state.partition, state.depth, graph);
    State newStateX = newState(n, state.partition, state.depth + 1, state.cX + 1, state.cY, newWeightX);
    
    if (newStateX.cX <= n - a) { // Ensure there is still room in subset X
        if (newWeightX < bestState->weight) { // Prune if current weight is worse than best weight
            int lowerBound = newWeightX + computeLowerBound(newStateX.depth, n, newStateX.partition, graph);
            if (lowerBound < bestState->weight) // Prune if lower bound is worse than best weight
                #pragma omp task
                {
                    bb_dfs(n, a, graph, newStateX, bestState, recCalls); // Recursion
                }
        }
    }

    // Branch where vertex idx is assigned to subset Y
    state.partition[state.depth] = 1;
    int newWeightY = state.weight + getWeightChange(state.partition, state.depth, graph);
    State newStateY = newState(n, state.partition, state.depth + 1, state.cX, state.cY + 1, newWeightY);

    if (newStateY.cY <= a) { // Ensure there is still room in subset Y
        if (newWeightY < bestState->weight) { // Prune if current weight is worse than best weight
            int lowerBound = newWeightY + computeLowerBound(newStateY.depth, n, newStateY.partition, graph);
            if (lowerBound < bestState->weight) // Prune if lower bound is worse than best weight
                #pragma omp task
                {
                    bb_dfs(n, a, graph, newStateY, bestState, recCalls); // Recursion
                }
        }
    }
    #pragma omp taskwait
}

// Function for finding the minimum cut of a graph
Solution findMinimumCut(Instance *instance, int numThreads) {
    int n = instance->n;
    int a = instance->a;
    int **graph = instance->graph;

    State state = initialState(n); // Initialize the first state
    State bestState = initialBestState(n); // Initialize the best state

    int recCalls = 0;
    double start_time = omp_get_wtime(); // Start timing execution
    
    #pragma omp parallel num_threads(numThreads)
    {
        #pragma omp single
        {
            bb_dfs(n, a, graph, state, &bestState, &recCalls);
        }
    }

    double end_time = omp_get_wtime(); // End timing execution
    double time_taken = (double)end_time - start_time; // Calculate time taken

    Solution solution;
    solution.partition = bestState.partition;
    solution.minWeight = bestState.weight;
    solution.recCalls = recCalls;
    solution.time = time_taken;

    return solution;
}

// Print the computed solution
void printSolution(Solution solution, int n) {
    printf("**************************************************\n");
    printf("Minimum cut: %d\n", solution.minWeight);
    printf("Recursive calls: %d\n", solution.recCalls);
    printf("Time taken: %f\n", solution.time);
    printf("**************************************************\n");
}