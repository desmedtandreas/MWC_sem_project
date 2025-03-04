#ifndef STATE_H
#define STATE_H

/**
 * @brief Represents a state in the search space.
 */
typedef struct {
    int* partition;    ///< Partition of the elements.
    int depth;         ///< Depth of the state in the search tree.
    int cX;            ///< Amount of elements in the X subset.
    int cY;            ///< Amount of elements in the Y subset. 
    int weight;        ///< Weight associated with the state.
    int instanceSize;  ///< Instance size of the problem (for memory management).
} State;

/**
 * @brief Creates a new state.
 * 
 * @param size Size of the partition.
 * @param partition Partition of the elements.
 * @param depth Depth of the state in the search tree.
 * @param cX Amount of elements in the X subset.
 * @param cY Amount of elements in the Y subset.
 * @param weight Weight associated with the state.
 * @return New state.
 */
State newState(int size, int* partition, int depth, int cX, int cY, int weight);

/**
 * @brief Creates an initial starting state.
 * 
 * @param n Size of the partition.
 * @return New state.
 */
State initialState(int n);

/**
 * @brief Creates an initial best state.
 * 
 * @param n Size of the partition.
 * @return An initial state
 */
State initialBestState(int n);

/**
 * @brief Copies a state.
 * 
 * Makes a deep copy of a state, allocating new memory for the partition.
 * 
 * @param s State to be copied.
 * @return Copy of the given state.
 */
State copyState(int size, State s);

/**
 * @brief Print a state.
 * 
 * Prints a state and its parameters.
 * Mainly used for debugging.
 * 
 * @param s State to be printed.
 */
void printState(State s);

#endif
    