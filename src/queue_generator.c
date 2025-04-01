#include <stdio.h>
#include <stdlib.h>
#include "state.h"

# define MAX_QUEUE_SIZE 1000000

StateArray bfs_initialstates(int n, int a, int **graph, int numThreads) {
    
    int front = 0;
    int rear = 0;
    State* queue = (State*)malloc(MAX_QUEUE_SIZE * sizeof(State));

    queue[rear++] = initialState(n);

    while(front < rear) {
        int levelSize = rear - front;

        if (levelSize >= numThreads) {
            State* initialStates = (State*)malloc(levelSize * sizeof(State));
            for (int i = 0; i < levelSize; i++) {
                initialStates[i] = copyState(n, queue[front + i]);
            }
            StateArray stateArray = {initialStates, levelSize};
            free(queue);
            return stateArray;
        }

        for (int i = 0; i < levelSize; i++) {
            State current = queue[front++];
            if (current.depth == n) {
                continue;
            }
            if (current.cX + 1 <= n - a) {
                current.partition[current.depth] = 0;
                int newWeightX = current.weight + getWeightChange(current.partition, current.depth, graph);
                State newStateX = newState(n, current.partition, current.depth + 1, current.cX + 1, current.cY, newWeightX);
                queue[rear++] = newStateX;
            }
            if (current.cY + 1 <= a) {
                current.partition[current.depth] = 1;
                int newWeightY = current.weight + getWeightChange(current.partition, current.depth, graph);
                State newStateY = newState(n, current.partition, current.depth + 1, current.cX, current.cY + 1, newWeightY);
                queue[rear++] = newStateY;
            }
        }
    }
    
    State* initialStates = (State*)malloc(rear * sizeof(State));
    for (int i = 0; i < rear; i++) {
        initialStates[i] = copyState(n, queue[i]);
    }
    StateArray stateArray = {initialStates, rear};
    return stateArray;
}