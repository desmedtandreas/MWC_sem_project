#ifndef STATE_H
#define STATE_H

typedef struct {
    int* partition;
    int depth;
    int cX;
    int cY;
    int weight;
    int instanceSize;
} State;

typedef struct {
    State** states;
    int count;
} StateArray;

/**
 * @brief Creates a new state (heap-allocated).
 * 
 * @param size Size of the partition.
 * @param partition Partition array.
 * @param depth Current depth in search tree.
 * @param cX Number of elements in subset X.
 * @param cY Number of elements in subset Y.
 * @param weight Weight of the current state.
 * @return Pointer to the new state.
 */
State* newState(int size, const int* partition, int depth, int cX, int cY, int weight);

/**
 * @brief Creates the initial state (heap-allocated).
 * 
 * @param n Problem size.
 * @return Pointer to the initial state.
 */
State* initialState(int n);

/**
 * @brief Creates an initial best state (heap-allocated).
 * 
 * @param n Problem size.
 * @return Pointer to the initial best state.
 */
State* initialBestState(int n);

/**
 * @brief Copies a state deeply (heap-allocated).
 * 
 * @param size Size of the partition.
 * @param s State to copy.
 * @return Pointer to the copied state.
 */
State* copyState(int size, const State* s);

/**
 * @brief Prints a state (for debugging).
 * 
 * @param s Pointer to the state to print.
 */
void printState(const State* s);

/**
 * @brief Frees a state (partition + struct).
 * 
 * @param s Pointer to the state to free.
 */
void freeState(State* s);

#endif
    