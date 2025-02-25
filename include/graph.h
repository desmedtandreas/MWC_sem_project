#ifndef GRAPH_H
#define GRAPH_H

#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>

// Function to free memory of a dynamically allocated graph
void freeGraph(int** graph, int n);

// Function to print adjacency matrix
void printGraph(int** graph, int n);

// Function to read graph from file
int** readGraphFromFile(const char* filename, int n);

#endif // GRAPH_H