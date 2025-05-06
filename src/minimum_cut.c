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

#define PARALLEL_THRESHOLD 5

int getWeightChange(int* partition, int idx, int** graph) {
    int weight = 0;
    if (partition[idx] == 1) {
        for (int i = 0; i < idx; i++) {
            if (partition[i] == 0) weight += graph[idx][i];
        }
    } else if (partition[idx] == 0) {
        for (int i = 0; i < idx; i++) {
            if (partition[i] == 1) weight += graph[idx][i];
        }
    }
    return weight;
}

int computeLowerBound(int idx, int n, int *partition, int **graph) {
    int lowerBound = 0;
    for (int i = idx; i < n; i++) {
        int costIfX = 0, costIfY = 0;
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

void bb_dfs(int n, int a, int **graph, State *state, State **bestState, int *recCalls) {
    #pragma omp atomic
    (*recCalls)++;

    if (state->depth == n) {
        #pragma omp critical
        {
            if (state->weight < (*bestState)->weight) {
                freeState(*bestState);
                *bestState = copyState(n, state);
            }
        }
        return;
    }

    int idx = state->depth;
    int saved = state->partition[idx];

    state->partition[idx] = 0;
    int newWeightX = state->weight + getWeightChange(state->partition, idx, graph);
    if (state->cX + 1 <= n - a && newWeightX < (*bestState)->weight) {
        State *newStateX = newState(n, state->partition, idx + 1, state->cX + 1, state->cY, newWeightX);
        int lowerBound = newWeightX + computeLowerBound(newStateX->depth, n, newStateX->partition, graph);
        if (lowerBound < (*bestState)->weight) {
            if (state->depth < PARALLEL_THRESHOLD) {
                #pragma omp task shared(bestState, recCalls)
                {
                    bb_dfs(n, a, graph, newStateX, bestState, recCalls);
                    freeState(newStateX);
                }
            } else {
                bb_dfs(n, a, graph, newStateX, bestState, recCalls);
                freeState(newStateX);
            }
        } else {
            freeState(newStateX);
        }
    }

    state->partition[idx] = 1;
    int newWeightY = state->weight + getWeightChange(state->partition, idx, graph);
    if (state->cY + 1 <= a && newWeightY < (*bestState)->weight) {
        State *newStateY = newState(n, state->partition, idx + 1, state->cX, state->cY + 1, newWeightY);
        int lowerBound = newWeightY + computeLowerBound(newStateY->depth, n, newStateY->partition, graph);
        if (lowerBound < (*bestState)->weight) {
            if (state->depth < PARALLEL_THRESHOLD) {
                #pragma omp task shared(bestState, recCalls)
                {
                    bb_dfs(n, a, graph, newStateY, bestState, recCalls);
                    freeState(newStateY);
                }
            } else {
                bb_dfs(n, a, graph, newStateY, bestState, recCalls);
                freeState(newStateY);
            }
        } else {
            freeState(newStateY);
        }
    }

    state->partition[idx] = saved;

    #pragma omp taskwait
}

Solution findMinimumCut(Instance *instance, int numThreads) {
    int n = instance->n;
    int a = instance->a;
    int **graph = instance->graph;

    State *state = initialState(n);
    State *bestState = initialBestState(n);

    int recCalls = 0;
    double start_time = omp_get_wtime();

    #pragma omp parallel num_threads(numThreads)
    {
        #pragma omp single
        {
            bb_dfs(n, a, graph, state, &bestState, &recCalls);
        }
    }

    freeState(state);

    double end_time = omp_get_wtime();
    double time_taken = end_time - start_time;

    Solution solution;
    solution.partition = bestState->partition;
    solution.minWeight = bestState->weight;
    solution.recCalls = recCalls;
    solution.time = time_taken;

    free(bestState); // partition is owned by solution now

    return solution;
}

void printSolution(Solution solution, int n) {
    printf("**************************************************\n");
    printf("Minimum cut: %d\n", solution.minWeight);
    printf("Recursive calls: %d\n", solution.recCalls);
    printf("Time taken: %f\n", solution.time);
    printf("**************************************************\n");
}
