#ifndef STATE_H
#define STATE_H

typedef struct {
    int* partition;
    int depth;
    int cX;
    int cY;
    int weight;
    int instanceSize;
} State;

State newState(int size, int* partition, int depth, int cX, int cY, int weight);

State initialState(int n);

State initialBestState(int n);

State copyState(int size, State s);

void printState(State s);

#endif
    