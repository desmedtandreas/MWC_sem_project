#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include <string.h>
#include "graph.h"
#include "instance.h"
#include "minimum_cut.h"

int* bestState;
int bestWeight;

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

void bb_dfs(int idx, int n, int a, int **graph, int *partition,
    int cX, int cY, int w, int *recCalls) {

    (*recCalls)++;

    // If all vertices have been assigned, update the best solution if needed.
    if (idx == n) {
        if (w < bestWeight) {
            bestWeight = w;
            memcpy(bestState, partition, n * sizeof(int));
        }
        return;
    }

    // Compute a lower bound based on the unassigned vertices.
    int lowerBound = w + computeLowerBound(idx, n, partition, graph);
    if (lowerBound >= bestWeight) return;

    // Branch where vertex idx is assigned to subset X (if there's still room).
    if (cX < a) {
    // Allocate a new partition array for this branch.
        int *newPartition = malloc(n * sizeof(int));
        memcpy(newPartition, partition, n * sizeof(int));
        newPartition[idx] = 0;  // assign to X

        int newWeight = w;
        // Calculate the added weight due to edges connecting to already assigned Y vertices.
        for (int i = 0; i < idx; i++) {
            if (newPartition[i] == 1) newWeight += graph[idx][i];
        }

        if (newWeight < bestWeight) {
            bb_dfs(idx + 1, n, a, graph, newPartition, cX + 1, cY, newWeight, recCalls);
        }

        free(newPartition);  // Free the copy after the recursive call returns.
    }

    // Branch where vertex idx is assigned to subset Y (if there's still room).
    if (cY < (n - a)) {
        int *newPartition = malloc(n * sizeof(int));
        memcpy(newPartition, partition, n * sizeof(int));
        newPartition[idx] = 1;  // assign to Y

        int newWeight = w;
        // Calculate the added weight due to edges connecting to already assigned X vertices.
        for (int i = 0; i < idx; i++) {
            if (newPartition[i] == 0) newWeight += graph[idx][i];
        }

        if (newWeight < bestWeight) {
            bb_dfs(idx + 1, n, a, graph, newPartition, cX, cY + 1, newWeight, recCalls);
        }

    free(newPartition);
    }
}

Solution findMinimumCut(Instance *instance) {
    int n = instance->n;
    int a = instance->a;
    int **graph = instance->graph;

    // Allocate and initialize the partition array. 
    // All vertices start unassigned (-1).
    int *partition = (int *)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        partition[i] = -1;
    }

    int recCalls = 0;
    clock_t start_time = clock();

    bb_dfs(0, n, a, graph, partition, 0, 0, 0, &recCalls);

    clock_t end_time = clock();
    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    Solution solution;
    solution.partition = bestState;
    solution.minWeight = bestWeight;
    solution.recCalls = recCalls;
    solution.time = time_taken;

    free(partition);
    return solution;
}

void printSolution(Solution solution, int n) {
    printf("**************************************************\n");
    printf("Minimum cut: %d\n", solution.minWeight);
    printf("Recursive calls: %d\n", solution.recCalls);
    printf("Time taken: %f\n", solution.time);
    printf("**************************************************\n");
}