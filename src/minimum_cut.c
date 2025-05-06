#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <omp.h>
#include <math.h>

#include "state.h"
#include "graph.h"
#include "instance.h"
#include "minimum_cut.h"

#define MAX_QUEUE_SIZE 1000000

int getWeightChange(const int *partition, int idx, int **graph) {
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

int computeLowerBound(int idx, int n, const int *partition, int **graph) {
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

    if (state->cX + 1 <= n - a) {
        state->partition[idx] = 0;
        int newWeightX = state->weight + getWeightChange(state->partition, idx, graph);
        if (newWeightX < (*bestState)->weight) {
            State *newStateX = newState(n, state->partition, idx + 1, state->cX + 1, state->cY, newWeightX);
            int lowerBound = newWeightX + computeLowerBound(newStateX->depth, n, newStateX->partition, graph);
            if (lowerBound < (*bestState)->weight)
                bb_dfs(n, a, graph, newStateX, bestState, recCalls);
            freeState(newStateX);
        }
    }

    if (state->cY + 1 <= a) {
        state->partition[idx] = 1;
        int newWeightY = state->weight + getWeightChange(state->partition, idx, graph);
        if (newWeightY < (*bestState)->weight) {
            State *newStateY = newState(n, state->partition, idx + 1, state->cX, state->cY + 1, newWeightY);
            int lowerBound = newWeightY + computeLowerBound(newStateY->depth, n, newStateY->partition, graph);
            if (lowerBound < (*bestState)->weight)
                bb_dfs(n, a, graph, newStateY, bestState, recCalls);
            freeState(newStateY);
        }
    }

    state->partition[idx] = saved;
}

StateArray bfs_initialstates(int n, int a, int **graph, int enoughStates) {
    State **queue = (State **)malloc(MAX_QUEUE_SIZE * sizeof(State *));
    int front = 0, rear = 0;

    queue[rear++] = initialState(n);

    while (front < rear) {
        int levelSize = rear - front;

        if (levelSize >= enoughStates) {
            StateArray result = { &queue[front], levelSize };
            return result;
        }

        for (int i = 0; i < levelSize; i++) {
            State *current = queue[front++];
            if (current->depth == n) continue;

            int idx = current->depth;

            if (current->cX + 1 <= n - a) {
                current->partition[idx] = 0;
                int newWeightX = current->weight + getWeightChange(current->partition, idx, graph);
                State *newStateX = newState(n, current->partition, idx + 1, current->cX + 1, current->cY, newWeightX);
                queue[rear++] = newStateX;
            }

            if (current->cY + 1 <= a) {
                current->partition[idx] = 1;
                int newWeightY = current->weight + getWeightChange(current->partition, idx, graph);
                State *newStateY = newState(n, current->partition, idx + 1, current->cX, current->cY + 1, newWeightY);
                queue[rear++] = newStateY;
            }

            current->partition[idx] = -1;
        }
    }

    StateArray result = { queue, rear };
    return result;
}

Solution findMinimumCut(Instance *instance, int numThreads, int enoughStates) {
    int n = instance->n;
    int a = instance->a;
    int **graph = instance->graph;

    State *bestState = initialBestState(n);
    int recCalls = 0;
    double start_time = omp_get_wtime();

    StateArray initialStates = bfs_initialstates(n, a, graph, enoughStates);

    #pragma omp parallel for num_threads(numThreads)
    for (int i = 0; i < initialStates.count; i++) {
        bb_dfs(n, a, graph, initialStates.states[i], &bestState, &recCalls);
    }

    double end_time = omp_get_wtime();
    double time_taken = end_time - start_time;

    Solution solution;
    solution.partition = bestState->partition;
    solution.minWeight = bestState->weight;
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
