#ifndef DATA_H
#define DATA_H

/**
 * @brief Represents a data structure.
 */
typedef struct Data {
    const char* filename;   ///< Name of the file containing the graph.
    int a;                  ///< Maximum number of allowed vertices in one of the subsets.
} Data;

#endif