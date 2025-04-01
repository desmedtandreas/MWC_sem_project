#include <stdio.h>
#include <graph.h>
#include <data.h>
#include <instance.h>
#include <limits.h>
#include <state.h>
#include <minimum_cut.h>
#include <mpi.h>

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
    int rank;
    int processes;

    // Create a graph instance from the file and parameter
    Instance* instance = createInstance(data[instanceNumber].filename, data[instanceNumber].a);

    MPI_Init(&argc, &argv);

    MPI_Comm_rank(MPI_COMM_WORLD, &rank);
    MPI_Comm_size(MPI_COMM_WORLD, &processes);

    if (rank == 0) {
        printf("Number of processes: %d\n", processes);
        printf("Instance number: %d\n", instanceNumber + 1);
    }
    else {
        printf("Process %d started\n", rank);
    }

    // Free the instance
    freeInstance(instance);

    MPI_Finalize();

    return 0;
}