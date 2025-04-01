#ifndef MPI_SEND_RECEIVE_H
#define MPI_SEND_RECEIVE_H

#include <state.h>

void send_state(State state, int rank);

void receive_state(State state, int rank);

#endif