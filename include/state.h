#ifndef STATE_H
#define STATE_H

/**
 * @brief Represents a state in the search space.
 */
typedef struct {
    int* partition;     ///< Partition of the elements.
    int depth;          ///< Depth of the state in the search tree.
    int cX;             ///< Amount of elements in the X subset.
    int cY;             ///< Amount of elements in the Y subset. 
    int weight;         ///< Weight associated with the state.
    int instanceSize;   ///< Instance size of the problem (for memory management).
} State;

/**
 * @brief Creates a new state on the heap.
 *
 * @param size Size of the partition.
 * @param partition Partition of the elements.
 * @param depth Depth of the state in the search tree.
 * @param cX Amount of elements in the X subset.
 * @param cY Amount of elements in the Y subset.
 * @param weight Weight associated with the state.
 * @return Pointer to the new state.
 */
State* newState(int size, const int* partition, int depth, int cX, int cY, int weight);

/**
 * @brief Creates an initial starting state on the heap.
 *
 * @param n Size of the partition.
 * @return Pointer to the initial state.
 */
State* initialState(int n);

/**
 * @brief Creates an initial best state on the heap.
 *
 * @param n Size of the partition.
 * @return Pointer to the initial best state.
 */
State* initialBestState(int n);

/**
 * @brief Copies a state deeply (on the heap).
 *
 * @param size Size of the partition.
 * @param s Pointer to the state to copy.
 * @return Pointer to the copied state.
 */
State* copyState(int size, const State* s);

/**
 * @brief Prints the state to stdout (for debugging).
 *
 * @param s Pointer to the state to print.
 */
void printState(const State* s);

/**
 * @brief Frees the memory allocated for a heap-based state.
 *
 * @param s Pointer to the state to free.
 */
void freeState(State* s);

#endif
