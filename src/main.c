#include <stdio.h>
#include <graph.h>
#include <data.h>
#include <instance.h>
#include <minimum_cut.h>

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
        // {"graphdata/graf_32_22.txt", 10},
        // {"graphdata/graf_32_25.txt", 12},
        // {"graphdata/graf_35_25.txt", 12},
        // {"graphdata/graf_35_25.txt", 17},
        // {"graphdata/graf_40_8.txt", 15},
        // {"graphdata/graf_40_8.txt", 20},
        // {"graphdata/graf_40_15.txt", 15},
        // {"graphdata/graf_40_15.txt", 20},
        // {"graphdata/graf_40_25.txt", 20},
    };

    Instance* instance = createInstance(data[10].filename, data[10].a);

    Solution solution = findMinimumCut(instance);

    printf("Minimum cut: %d\n", solution.minWeight);
    printf("Recursive calls: %d\n", solution.recCalls);

    //freeInstance(instance);
    return 0;
}