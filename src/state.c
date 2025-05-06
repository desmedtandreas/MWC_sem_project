#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "state.h"

State* newState(int size, const int *partition, int depth, int cX, int cY, int weight) {
    State *s = (State *)malloc(sizeof(State));
    if (!s) return NULL;

    s->partition = (int *)malloc(size * sizeof(int));
    if (!s->partition) {
        free(s);
        return NULL;
    }

    memcpy(s->partition, partition, size * sizeof(int));

    s->depth = depth;
    s->cX = cX;
    s->cY = cY;
    s->weight = weight;
    s->instanceSize = size;
    return s;
}

State* initialState(int n) {
    State *s = (State *)malloc(sizeof(State));
    if (!s) return NULL;

    s->partition = (int *)malloc(n * sizeof(int));
    if (!s->partition) {
        free(s);
        return NULL;
    }

    for (int i = 0; i < n; ++i)
        s->partition[i] = -1;

    s->depth = 0;
    s->cX = 0;
    s->cY = 0;
    s->weight = 0;
    s->instanceSize = n;
    return s;
}

State* initialBestState(int n) {
    State *s = initialState(n);
    if (s) s->weight = INT_MAX;
    return s;
}

State* copyState(int size, const State *src) {
    return newState(size, src->partition, src->depth, src->cX, src->cY, src->weight);
}

void printState(const State *s) {
    printf("State: Partition: ");
    for (int i = 0; i < s->depth; ++i)
        printf("%d ", s->partition[i]);
    printf(" | Depth: %d  cX: %d  cY: %d  Weight: %d\n",
           s->depth, s->cX, s->cY, s->weight);
}

void freeState(State *s) {
    if (!s) return;
    free(s->partition);
    free(s);
}