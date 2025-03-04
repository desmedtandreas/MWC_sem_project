#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

/**
 * @brief Frees the memory allocated for a graph.
 * 
 * @param graph Graph to be freed.
 * @param n Number of vertices in the graph.
 */
void freeGraph(int** graph, int n);

/**
 * @brief Prints a graph.
 * 
 * Prints a graph represented as an adjacency matrix.
 * Mainly used for debugging.
 * 
 * @param graph Graph to be printed.
 * @param n Number of vertices in the graph.
 */
void printGraph(int** graph, int n);

/**
 * @brief Reads a graph from a file.
 * 
 * Reads a graph from a file and returns it as an adjacency matrix.
 * 
 * @param filename Name of the file containing the graph.
 * @param n Number of vertices in the graph.
 * @return Graph represented as an adjacency matrix.
 */
int** readGraphFromFile(const char* filename, int n);

#endif