#ifndef STATE_H
#define STATE_H

#include <instance.h>

typedef struct State {
    int* partition;
    int count_x;
    int count_y;
    int weight;
} State;

void printState(State state);

void copyState(State* dest, const State* src, int n);

State initialize_state(Instance* instance);

#endif