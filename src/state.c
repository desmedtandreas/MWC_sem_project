#include <stdio.h>
#include <stdlib.h>
#include <state.h>
#include <string.h>
#include <limits.h>

State newState(int size, int* partition, int depth, int cX, int cY, int weight) {
    State s;
    int* newPartition = (int*)malloc(size * sizeof(int));
    memcpy(newPartition, partition, size * sizeof(int));
    s.partition = newPartition;
    s.depth = depth;
    s.cX = cX;
    s.cY = cY;
    s.weight = weight;
    return s;
}

State initialState(int n) {
    State s;
    s.partition = (int*)malloc(n * sizeof(int));
    for (int i = 0; i < n; i++) {
        s.partition[i] = -1;
    }
    s.depth = 0;
    s.cX = 0;
    s.cY = 0;
    s.weight = 0;
    return s;
}

State initialBestState(int n) {
    State s;
    s.partition = (int*)malloc(n * sizeof(int));
    s.partition = NULL;
    s.depth = 0;
    s.cX = 0;
    s.cY = 0;
    s.weight = INT_MAX;
    return s;
}

State copyState(int size, State s) {
    State copiedState = newState(size, s.partition, s.depth, s.cX, s.cY, s.weight);
    return copiedState;
}

void printState(State s) {
    printf("State: ");
    printf("Partition: ");
    for (int i = 0; i < s.depth; i++) {
        printf("%d ", s.partition[i]);
    }
    printf("Depth: %d cX: %d cY: %d Weight: %d\n", s.depth, s.cX, s.cY, s.weight);
    printf("\n");
}