#include <stdio.h>
#include <stdlib.h>
#include <state.h>
#include <instance.h>

int computeWeight(State* state, Instance* instance) {
    int weight = 0;
    int n = instance->n;
    int** graph = instance->graph;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            if (state->partition[i] != state->partition[j]) {
                weight += graph[i][j];
            }
        }
    }
    return weight;
}

void printState(State state) {
    printf("Partition: ");
    for (int i = 0; i < state.count_x + state.count_y; i++) {
        printf("%d ", state.partition[i]);
    }
    printf("\n");
    printf("Count X: %d\n", state.count_x);
    printf("Count Y: %d\n", state.count_y);
    printf("Weight: %d\n", state.weight);
}

State initialState(Instance* instance) {
    int n = instance->n;
    int a = instance->a;
    State state;
    state.partition = (int*)malloc(instance->n * sizeof(int));
    for (int i = 0; i < n; i++) state.partition[i] = i < a ? 1 : 0;
    state.count_x = a;
    state.count_y = n - a;
    state.weight = computeWeight(&state, instance);
    return state;
}

void copyState(State* dest, const State* src, int n) {
    dest->partition = (int*)malloc(n * sizeof(int));
    if (dest->partition == NULL) {
        perror("Failed to allocate memory for partition");
        exit(EXIT_FAILURE);
    }
    memcpy(dest->partition, src->partition, n * sizeof(int));
    dest->count_x = src->count_x;
    dest->count_y = src->count_y;
    dest->weight = src->weight;
}
