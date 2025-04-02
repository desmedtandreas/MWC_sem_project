// #include <mpi.h>
// #include <stddef.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <string.h>
// #include <state.h>

// void send_state(State* state, int rank) {
//     // Create a derived datatype for the partition array
//     MPI_Datatype partition_type;

//     printf("Invalid instanceSize on send: %d\n", state->instanceSize);
        
//     MPI_Type_contiguous(state->instanceSize, MPI_INT, &partition_type);
//     MPI_Type_commit(&partition_type);

//     MPI_Aint displacements[6];
//     int block_lengths[6] = {1, 1, 1, 1, 1, 1};  // Each field has length 1
//     MPI_Datatype types[6] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, partition_type}; // Field types

//     // Get displacements (offsets) for each field in the struct
//     displacements[0] = offsetof(State, partition);
//     displacements[1] = offsetof(State, depth);
//     displacements[2] = offsetof(State, cX);
//     displacements[3] = offsetof(State, cY);
//     displacements[4] = offsetof(State, weight);
//     displacements[5] = offsetof(State, instanceSize);

//     MPI_Datatype state_type;
//     MPI_Type_create_struct(6, block_lengths, displacements, types, &state_type);
//     MPI_Type_commit(&state_type);  // Commit the datatype

//     // Send the struct
//     MPI_Send(state, 1, state_type, 1, 0, MPI_COMM_WORLD);
//     printf("Rank %d: Sent state\n", rank);

//     // Free the derived types
//     MPI_Type_free(&state_type);
//     MPI_Type_free(&partition_type);
// }

// void recieve_state(State* state, int rank) {
//     // Create a derived datatype for the partition array
//     MPI_Datatype partition_type;

//     printf("InstanceSize on recieve: %d\n", state->instanceSize);

//     MPI_Type_contiguous(state->instanceSize, MPI_INT, &partition_type);
//     MPI_Type_commit(&partition_type);

//     MPI_Aint displacements[6];
//     int block_lengths[6] = {1, 1, 1, 1, 1, 1};  // Each field has length 1
//     MPI_Datatype types[6] = {MPI_INT, MPI_INT, MPI_INT, MPI_INT, MPI_INT, partition_type}; // Field types

//     // Get displacements (offsets) for each field in the struct
//     displacements[0] = offsetof(State, partition);
//     displacements[1] = offsetof(State, depth);
//     displacements[2] = offsetof(State, cX);
//     displacements[3] = offsetof(State, cY);
//     displacements[4] = offsetof(State, weight);
//     displacements[5] = offsetof(State, instanceSize);

//     MPI_Datatype state_type;
//     MPI_Type_create_struct(6, block_lengths, displacements, types, &state_type);
//     MPI_Type_commit(&state_type);  // Commit the datatype

//     // Receive the struct
//     MPI_Recv(state, 1, state_type, 0, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
//     printf("Rank %d: Received state\n", rank);

//     // Allocate memory for the partition array
//     state->partition = (int*)malloc(state->instanceSize * sizeof(int));

//     // Print received state
//     printf("Depth: %d, cX: %d, cY: %d, Weight: %d, InstanceSize: %d\n",
//             state->depth, state->cX, state->cY, state->weight, state->instanceSize);

//     // Print partition array
//     for (int i = 0; i < state->instanceSize; i++) {
//         printf("partition[%d] = %d\n", i, state->partition[i]);
//     }

//     // Free the derived types
//     MPI_Type_free(&state_type);
//     MPI_Type_free(&partition_type);
// }