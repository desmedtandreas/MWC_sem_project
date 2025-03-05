#include <stdio.h>
#include <graph.h>
#include <data.h>
#include <instance.h>
#include <limits.h>
#include <state.h>
#include <minimum_cut.h>

int main(int argc, char *argv[]) {
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
    int amountInstances = atoi(argv[1]);
    for (int i = 0; i < amountInstances; i++) {
        int numThreads = atoi(argv[2]);
        
        // Create a graph instance from the file and parameter
        Instance* instance = createInstance(data[i].filename, data[i].a);

        // Compute the minimum cut of the graph
        Solution solution = findMinimumCut(instance, numThreads);

        // Print the computed solution
        printSolution(solution, instance->n);

        // Free the instance
        freeInstance(instance);
    }
    return 0;
}