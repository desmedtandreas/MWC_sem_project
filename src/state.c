#include <stdio.h>
#include <stdlib.h>
#include <state.h>
#include <string.h>
#include <limits.h>

// Create a new state
State newState(int size, int* partition, int depth, int cX, int cY, int weight) {
    State s;
    memcpy(s.partition, partition, size * sizeof(int));
    s.depth = depth;
    s.cX = cX;
    s.cY = cY;
    s.weight = weight;
    s.instanceSize = size;
    return s;
}

// Create the initial state
State initialState(int n) {
    State s;
    for (int i = 0; i < n; i++) {
        s.partition[i] = -1; // all elements are unassigned (-1)
    }
    s.depth = 0;
    s.cX = 0;
    s.cY = 0;
    s.weight = 0;
    return s;
}

// Create the initial best state
State initialBestState(int n) {
    State s;
    for (int i = 0; i < n; i++) {
        s.partition[i] = -1; // all elements are unassigned (-1)
    }
    s.depth = 0;
    s.cX = 0;
    s.cY = 0;
    s.weight = INT_MAX; // set to maximum value
    return s;
}

// Copy a state (deep copy = new memory allocation)
State copyState(int size, State s) {
    State copiedState = newState(size, s.partition, s.depth, s.cX, s.cY, s.weight);
    return copiedState;
}

// Print a state (for debugging)
void printState(State s) {
    printf("State: ");
    printf("Partition: ");
    for (int i = 0; i < s.depth; i++) {
        printf("%d ", s.partition[i]);
    }
    printf("Depth: %d cX: %d cY: %d Weight: %d\n", s.depth, s.cX, s.cY, s.weight);
    printf("\n");
}