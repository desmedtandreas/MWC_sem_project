#include <stdio.h>
#include <graph.h>
#include <mpi.h>
#include <omp.h>
#include <data.h>
#include <instance.h>
#include <limits.h>
#include <state.h>
#include <minimum_cut.h>
#include <queue_generator.h>

# define TAG_STATE 0
# define TAG_BEST_SOLUTION 1
# define TAG_RESULT 2
# define TAG_TERMINATE 3

int main(int argc, char* argv[]) {
    // Datasets with the filename and parameter a
    Data data[] = {
        {"graphdata/graf_10_5.txt", 5},
        {"graphdata/graf_10_6b.txt", 5},
        {"graphdata/graf_10_7.txt", 5},
        {"graphdata/graf_15_14.txt", 5},
        {"graphdata/graf_20_7.txt", 7},
        {"graphdata/graf_20_7.txt", 10},
        {"graphdata/graf_20_12.txt", 10},
        {"graphdata/graf_20_17.txt", 10},
        {"graphdata/graf_30_10.txt", 10},
        {"graphdata/graf_30_10.txt", 15},
        {"graphdata/graf_30_20.txt", 15},
        {"graphdata/graf_32_22.txt", 10},
        {"graphdata/graf_32_25.txt", 12},
        {"graphdata/graf_35_25.txt", 12},
        {"graphdata/graf_35_25.txt", 17},
        {"graphdata/graf_40_8.txt", 15},
        {"graphdata/graf_40_8.txt", 20},
        {"graphdata/graf_40_15.txt", 15},
        {"graphdata/graf_40_15.txt", 20},
        {"graphdata/graf_40_25.txt", 20},
    };

    // Looping through the entries in the dataset
    int instanceNumber = atoi(argv[1]) - 1;
    int numberOfStates = atoi(argv[2]);
    int numThreads = atoi(argv[3]);
    int bestSolution = INT_MAX;
    int rank, processes;
    double start, end;
    MPI_Status status;
    State state;
    int totalSize = sizeof(State);

    // Create a graph instance from the file and parameter
    Instance* instance = createInstance(data[instanceNumber].filename, data[instanceNumber].a);

    start = omp_get_wtime();
    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processes);

    if (rank == 0) {
        printf("Loaded graph: %s\n", data[instanceNumber].filename);
        printf("Number of vertices: %d\n", instance->n);
        printf("Parameter a: %d\n", data[instanceNumber].a);
        printf("Number of threads: %d\n", numThreads);
        printf("Number of processes: %d\n", processes);
        
        int numSlaves = processes - 1;
        int taskInProgress = 0;
        int slave;

        StateArray startingStates = bfs_initialstates(instance->n, instance->a, instance->graph, numberOfStates);
        slave = 1;
        while (startingStates.count > 0) {
            state = startingStates.states[startingStates.count - 1];

            MPI_Send(&state, totalSize, MPI_BYTE, slave, TAG_STATE, MPI_COMM_WORLD);
            MPI_Send(&bestSolution, 1, MPI_INT, slave, TAG_BEST_SOLUTION, MPI_COMM_WORLD);

            startingStates.count--;
            taskInProgress++;
            slave++;

            if (taskInProgress == numSlaves) {
                MPI_Recv(&bestSolution, 1, MPI_INT, MPI_ANY_SOURCE, TAG_RESULT, MPI_COMM_WORLD, &status);
                slave = status.MPI_SOURCE;
                taskInProgress--;
            }
        }

        for (slave= 1; slave <= numSlaves; slave++) {
            int term = -1;
            MPI_Send(&term, 1, MPI_INT, slave, TAG_TERMINATE, MPI_COMM_WORLD);
        }


        for (slave= 1; slave <= numSlaves; slave++) {
            MPI_Recv(&bestSolution, 1, MPI_INT, MPI_ANY_SOURCE, TAG_RESULT, MPI_COMM_WORLD, &status);
        }
    }
    else {
        int end = 0;

        while(!end) {
            MPI_Recv(&state, totalSize, MPI_BYTE, 0, MPI_ANY_TAG, MPI_COMM_WORLD, &status);
            if (status.MPI_TAG == TAG_TERMINATE) {
                MPI_Send(&bestSolution, 1, MPI_INT, 0, TAG_RESULT, MPI_COMM_WORLD);
                end = 1;
            } else if (status.MPI_TAG == TAG_STATE) {
                MPI_Recv(&bestSolution, 1, MPI_INT, 0, TAG_BEST_SOLUTION, MPI_COMM_WORLD, MPI_STATUS_IGNORE);

                bestSolution = findMinimumCut(instance, state, bestSolution, numThreads);

                MPI_Send(&bestSolution, 1, MPI_INT, 0, TAG_RESULT, MPI_COMM_WORLD);
            }
        }
    }

    // Free the instance
    freeInstance(instance);
    MPI_Finalize();

    if (rank == 0) {
        end = omp_get_wtime();
        printf("**************************************************\n");
        printf("Minimum cut: %d\n", bestSolution);
        printf("Time taken: %f\n", end - start);
        printf("**************************************************\n");
    }

    return 0;
}