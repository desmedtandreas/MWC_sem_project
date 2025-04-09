#include <stdio.h>
#include <stdlib.h>
#include <graph.h>
#include <instance.h>

// Load the size of the instance from a file
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

// Free the memory allocated for an instance
void freeInstance(Instance* instance) {
    freeGraph(instance->graph, instance->n);
    free(instance);
}

// Create an instance from a file
Instance* createInstance(const char* filename, int a) {
    int n = loadInstanceSize(filename);
    Instance* instance = (Instance*)malloc(sizeof(Instance));
    instance->n = n;
    instance->a = a;
    instance->graph = readGraphFromFile(filename, n);
    return instance;
}