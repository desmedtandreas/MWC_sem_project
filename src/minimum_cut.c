#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <instance.h>
#include <state.h>
#include <minimum_cut.h>
#include <limits.h>

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

    if (cX > a) return;
    if (w > bestState->weight) return;
    if (idx == n) {
        if ((cX == a || cY == a) && w < bestState->weight) {
            copyState(bestState, &state, n);
        }
        return; 
    }

    for (int i = n - 1; i > idx - 1; i--) {
        partition[i] = -1;
    }

    int lowerBound = w + computeLowerBound(partition, n, graph);
    if (lowerBound >= bestState->weight) return;

    partition[idx] = 0;
    int newWeightX = w;
    for (int i = 0; i < idx; i++) {
        if (partition[i] == 1) newWeightX += graph[idx][i];
    }
    State newStateX = {
        .partition = partition,
        .count_x = cX + 1,
        .count_y = cY,
        .weight = newWeightX
    };
    bb_dfs(idx + 1, newStateX, bestState, instance, recCalls);

    partition[idx] = 1;
    int newWeightY = w;
    for (int i = 0; i < idx; i++) {
        if (partition[i] == 0) newWeightY += graph[idx][i];
    }
    State newStateY = {
        .partition = partition,
        .count_x = cX,
        .count_y = cY + 1,
        .weight = newWeightY
    };
    bb_dfs(idx + 1, newStateY, bestState, instance, recCalls);
}

Solution findMinimumCut(Instance* instance) {
    State state = initialize_state(instance);
    State bestState = state;
    bestState.weight = 100000;
    int recCalls = 0;

    bb_dfs(0, state, &bestState, instance, &recCalls);

    printState(bestState);
    
    Solution solution = {
        .partition = bestState.partition,
        .minWeight = bestState.weight,
        .recCalls = recCalls
    };

    return solution;
}