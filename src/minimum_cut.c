#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <time.h>
#include "graph.h"
#include "instance.h"
#include "state.h"
#include "minimum_cut.h"

void printSolution(Solution solution, int n) {
    printf("**************************************************\n");
    // printf("Partition: ");
    // for (int i = 0; i < n; i++) {
    //     printf("%d ", solution.partition[i]);
    // }
    // printf("\n");
    printf("Minimum cut: %d\n", solution.minWeight);
    printf("Recursive calls: %d\n", solution.recCalls);
    printf("Time taken: %f\n", solution.time);
    printf("**************************************************\n");
}

int computeLowerBound(int* partition, int n, int** graph) {
    int lowerBound = 0;
    for (int i = 0; i < n; i++) {
        if (partition[i] == -1) {
            int add_to_x = 0, add_to_y = 0;
            for (int j = 0; j < n; j++) {
                if (partition[j] == 1) add_to_x += graph[i][j];
                if (partition[j] == 0) add_to_y += graph[i][j];
            }
            lowerBound += add_to_x < add_to_y ? add_to_x : add_to_y;
        }
    }
    return lowerBound;
}

void bb_dfs(int idx, State state, State* bestState, Instance* instance, int* recCalls) {
    (*recCalls)++;
    int* partition = state.partition;
    int cX = state.count_x;
    int cY = state.count_y;
    int w = state.weight;
    int a = instance->a;
    int n = instance->n;
    int** graph = instance->graph;

    if (cX > a || cY > (n - a)) return;
    if (w > bestState->weight) return;
    if (idx == n) {
        if (cX == a && cY == (n - a) && w < bestState->weight) {
            copyState(bestState, &state, n);
        }
        return;
    }

    int lowerBound = w + computeLowerBound(partition, n, graph);
    if (lowerBound >= bestState->weight) return;

    // Explore partitioning the current vertex into set X
    partition[idx] = 0;
    int newWeightX = w;
    for (int i = 0; i < idx; i++) {
        if (partition[i] == 1) newWeightX += graph[idx][i];
    }
    State newStateX = {
        .partition = (int*)malloc(n * sizeof(int)),
        .count_x = cX + 1,
        .count_y = cY,
        .weight = newWeightX
    };
    memcpy(newStateX.partition, partition, n * sizeof(int));
    bb_dfs(idx + 1, newStateX, bestState, instance, recCalls);
    free(newStateX.partition);

    // Explore partitioning the current vertex into set Y
    partition[idx] = 1;
    int newWeightY = w;
    for (int i = 0; i < idx; i++) {
        if (partition[i] == 0) newWeightY += graph[idx][i];
    }
    State newStateY = {
        .partition = (int*)malloc(n * sizeof(int)),
        .count_x = cX,
        .count_y = cY + 1,
        .weight = newWeightY
    };
    memcpy(newStateY.partition, partition, n * sizeof(int));
    bb_dfs(idx + 1, newStateY, bestState, instance, recCalls);
    free(newStateY.partition);
}

Solution findMinimumCut(Instance* instance) {
    State state = initialize_state(instance);
    State bestState = state;
    bestState.weight = INT_MAX;
    int recCalls = 0;

    clock_t start_time = clock();
    bb_dfs(0, state, &bestState, instance, &recCalls);
    clock_t end_time = clock();

    double time_taken = (double)(end_time - start_time) / CLOCKS_PER_SEC;

    Solution solution = {
        .partition = bestState.partition,
        .minWeight = bestState.weight,
        .recCalls = recCalls,
        .time = time_taken,
    };

    return solution;
}