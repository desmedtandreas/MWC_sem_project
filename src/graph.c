#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <string.h>
#include <graph.h>

// Create a dynamically allocated array of integers
int** createGraph(int n) { // double pointer for 2D array
    int** graph = (int**)malloc(n * sizeof(int*));
    for (int i = 0; i < n; i++) {
        graph[i] = (int*)malloc(n * sizeof(int));
    }
    return graph;
}

// Read adjacency matrix from a file
int** readGraph(FILE* file, int n) {
    int** matrix = createGraph(n); // Allocate memory for the graph

    fscanf(file, "%*d"); // Skip the first number (number of vertices)

    // Read adjacency matrix
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            fscanf(file, "%d", &matrix[i][j]);
        }
    }
    return matrix;
}

// Free memory of a dynamically allocated graph
void freeGraph(int** graph, int n) {
    for (int i = 0; i < n; i++) {
        free(graph[i]);
    }
    free(graph);
}

// Print adjacency matrix (for debugging)
void printGraph(int** matrix, int n) {
    printf("Adjacency Matrix (%d x %d):\n", n, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            printf("%3d ", matrix[i][j]);
        }
        printf("\n");
    }
}

// Read adjacency matrix from a file
int** readGraphFromFile(const char* filename, int n) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        return NULL;
    }
    
    int** graph = readGraph(file, n); 

    fclose(file);
    printf("Read graph from file: %s\n", filename);
    return graph;
}