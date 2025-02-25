#include <stdio.h>
#include <stdlib.h>
#include <state.h>
#include <instance.h>

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

State initialize_state(Instance* instance) {
    State state;
    state.partition = (int*)malloc(instance->n * sizeof(int));
    memset(state.partition, -1, instance->n * sizeof(int));
    state.count_x = 0;
    state.count_y = 0;
    state.weight = 0;
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
