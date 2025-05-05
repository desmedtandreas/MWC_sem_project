#include <stdio.h>
#include <stdlib.h>
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
void bb_dfs(int n, int a, int **graph, State *state, State* bestState, long long unsigned int *recCalls) {
    (*recCalls)++;

    if (state->depth == n) {
        if (state->weight < bestState->weight) {
            *bestState = copyState(n, *state);
        }
        return;
    }

    int currentDepth = state->depth;
    int originalWeight = state->weight;
    int originalCX = state->cX;
    int originalCY = state->cY;

    // --- Try assigning current vertex to subset X ---
    state->partition[currentDepth] = 0;
    int weightX = originalWeight + getWeightChange(state->partition, currentDepth, graph);
    state->weight = weightX;
    state->cX = originalCX + 1;
    state->depth = currentDepth + 1;

    if (state->cX <= n - a && weightX < bestState->weight) {
        int lowerBound = weightX + computeLowerBound(state->depth, n, state->partition, graph);
        if (lowerBound < bestState->weight) {
            bb_dfs(n, a, graph, state, bestState, recCalls);
        }
    }

    // Backtrack state
    state->depth = currentDepth;
    state->weight = originalWeight;
    state->cX = originalCX;

    // --- Try assigning current vertex to subset Y ---
    state->partition[currentDepth] = 1;
    int weightY = originalWeight + getWeightChange(state->partition, currentDepth, graph);
    state->weight = weightY;
    state->cY = originalCY + 1;
    state->depth = currentDepth + 1;

    if (state->cY <= a && weightY < bestState->weight) {
        int lowerBound = weightY + computeLowerBound(state->depth, n, state->partition, graph);
        if (lowerBound < bestState->weight) {
            bb_dfs(n, a, graph, state, bestState, recCalls);
        }
    }

    // Backtrack again
    state->depth = currentDepth;
    state->weight = originalWeight;
    state->cY = originalCY;
}

// Function for finding the minimum cut of a graph
Solution findMinimumCut(Instance *instance) {
    int n = instance->n;
    int a = instance->a;
    int **graph = instance->graph;

    State state = initialState(n); // Initialize the first state
    State bestState = initialBestState(n); // Initialize the best state

    unsigned long long int recCalls = 0;
    clock_t start_time = clock(); // Start timing execution

    bb_dfs(n, a, graph, &state, &bestState, &recCalls);

    clock_t end_time = clock(); // End timing execution
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

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
    printf("Recursive calls: %lld\n", solution.recCalls);
    printf("Time taken: %f\n", solution.time);
    printf("**************************************************\n");
}