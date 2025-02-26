#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include "state.h"
#include "graph.h"
#include "instance.h"
#include "minimum_cut.h"

int getWeightChange(int* partition, int idx, int** graph) {
    int weight = 0;
    if (partition[idx] == 1) {
        for (int i = 0; i < idx; i++) {
            if (partition[i] == 0) weight += graph[idx][i];  // updated to use weight
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
        lowerBound += (costIfX < costIfY ? costIfX : costIfY);
    }
    return lowerBound;
}

void bb_dfs(int n, int a, int **graph, State state, State* bestState, int *recCalls) {

    (*recCalls)++;

    // If all vertices have been assigned, update the best solution if needed.
    if (state.depth == n) {
        if (state.weight < bestState->weight) {
            *bestState = copyState(n, state);
        }
        return;
    }

    state.partition[state.depth] = 0;
    int newWeightX = state.weight + getWeightChange(state.partition, state.depth, graph);
    State newStateX = newState(n, state.partition, state.depth + 1, state.cX + 1, state.cY, newWeightX);
    
    if (newStateX.cX <= n - a) {
        if (newWeightX < bestState->weight) {
            int lowerBound = newWeightX + computeLowerBound(newStateX.depth, n, newStateX.partition, graph);
            if (lowerBound < bestState->weight)
                bb_dfs(n, a, graph, newStateX, bestState, recCalls);
        }
    }

    // Branch where vertex idx is assigned to subset Y (if there's still room).
    state.partition[state.depth] = 1;
    int newWeightY = state.weight + getWeightChange(state.partition, state.depth, graph);
    State newStateY = newState(n, state.partition, state.depth + 1, state.cX, state.cY + 1, newWeightY);

    if (newStateY.cY <= a) {
        if (newWeightY < bestState->weight) {
            int lowerBound = newWeightY + computeLowerBound(newStateY.depth, n, newStateY.partition, graph);
            if (lowerBound < bestState->weight)
                bb_dfs(n, a, graph, newStateY, bestState, recCalls);
        }
    }
}

Solution findMinimumCut(Instance *instance) {
    int n = instance->n;
    int a = instance->a;
    int **graph = instance->graph;

    State state = initialState(n);
    State bestState = initialBestState(n);

    int recCalls = 0;
    clock_t start_time = clock();

    bb_dfs(n, a, graph, state, &bestState, &recCalls);

    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    Solution solution;
    solution.partition = bestState.partition;
    solution.minWeight = bestState.weight;
    solution.recCalls = recCalls;
    solution.time = time_taken;

    return solution;
}

void printSolution(Solution solution, int n) {
    printf("**************************************************\n");
    printf("Minimum cut: %d\n", solution.minWeight);
    printf("Recursive calls: %d\n", solution.recCalls);
    printf("Time taken: %f\n", solution.time);
    printf("**************************************************\n");
}