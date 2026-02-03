#include <iostream>
#include <vector>
#include <chrono>
#include "ParallelPrefix.hpp"

using namespace std;
using namespace ParallelScan; 

using Data = vector<int>;

const int N = 100000000;

int main() {
    Data data(N, 1);
    data[0] = 10;
    Data prefix(N);

    // start timer
    auto start = chrono::steady_clock::now();

    ParallelSum<int> heap(&data);
    heap.compute(&prefix);

    // stop timer
    auto end = chrono::steady_clock::now();
    auto elapsed = chrono::duration<double, milli>(end - start).count();

    int check = 10;
    for (int elem : prefix) {
        if (elem != check++) {
            cout << "FAILED RESULT at " << check - 1 << endl;
            break;
        }
    }

    cout << "in " << elapsed << "ms" << endl;
    return 0;
}