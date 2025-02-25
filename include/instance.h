#ifndef INSTANCE_H
#define INSTANCE_H

#include <graph.h>

typedef struct Instance {
    int n;
    int a;
    int** graph;
} Instance;

void freeInstance(Instance* instance);

Instance* createInstance(const char* filename, int a);

#endif