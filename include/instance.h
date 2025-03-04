#ifndef INSTANCE_H
#define INSTANCE_H

#include <graph.h>

/**
 * @brief Represents a problem instance.
 */
typedef struct Instance {
    int n;          ///< Number of vertices.
    int a;          ///< Maximum number of allowed vertices in one of the subsets.
    int** graph;    ///< Graph of the instance represented as an adjacency matrix.
} Instance;

/**
 * @brief Frees the memory allocated for an instance.
 * 
 * @param instance Instance to be freed.
 */
void freeInstance(Instance* instance);

/**
 * @brief Creates a new instance.
 * 
 * @param filename Name of the file containing the graph.
 * @param a Maximum number of allowed vertices in one of the subsets.
 * @return Pointer to an instance.
 */
Instance* createInstance(const char* filename, int a);

#endif