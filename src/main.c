#include <stdio.h>
#include <graph.h>
#include <data.h>
#include <instance.h>
#include <limits.h>
#include <minimum_cut.h>

extern int* bestState;
extern int bestWeight;

int main() {

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

    for (int i = 0; i < 15; i++) {
        Instance* instance = createInstance(data[i].filename, data[i].a);

        bestState = (int*)malloc(instance->n * sizeof(int));
        bestWeight = INT_MAX;

        Solution solution = findMinimumCut(instance);
        printSolution(solution, instance->n);

        freeInstance(instance);
    }

    //freeInstance(instance);
    return 0;
}