#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <instance.h>

int loadInstanceSize(const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        fprintf(stderr, "Error: Could not open file %s\n", filename);
        exit(1);
    }

    int n;
    fscanf(file, "%d", &n);
    fclose(file);
    return n;
}

void freeInstance(Instance* instance) {
    freeGraph(instance->graph, instance->n);
    free(instance);
}

Instance* createInstance(const char* filename, int a) {
    int n = loadInstanceSize(filename);
    Instance* instance = (Instance*)malloc(sizeof(Instance));
    instance->n = n;
    instance->a = a;
    instance->graph = readGraphFromFile(filename, n);
    return instance;
}