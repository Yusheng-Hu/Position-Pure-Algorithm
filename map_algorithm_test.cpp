#include "windows.h" 
#include <chrono>
#include <functional>
#include <iostream>
#include <random>
#include <vector>
#include "map_perm_algorithms.h"

using namespace std;
using namespace chrono;

// 1. Modify the test framework to accept pre-generated data
long long runAlgorithmTest(const string &algorithmName, 
                         void (*algorithm)(const vector<int>&, vector<int>&),
                         const vector<vector<int>>& C_samples, 
                         int n, long long maxCount) {
    long long checksum = 0;
    vector<int> D(n); // Allocate D array only once at the beginning

    // Start timing: only includes algorithm core loop
    auto start = high_resolution_clock::now();

    for (long long count = 0; count < maxCount; ++count) {
        // Directly reference pre-generated samples without recalculation
        algorithm(C_samples[count % C_samples.size()], D);
        checksum += D[0] ^ D[n / 2] ^ D[n - 1]; 
    }

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<microseconds>(end - start);

    cout << algorithmName << " - n = " << n << ", count = " << maxCount << endl;
    cout << "Total Execution time: " << duration.count() / 1e6 << " seconds" << endl;
    cout << "Average time per iteration: " << (double)duration.count() / maxCount << " microseconds" << endl;
    cout << "Checksum result: " << checksum << endl; 
    cout << "----------------------------------------" << endl;

    return maxCount;
}

int main() {
    // Bind thread to CPU core 3 (0-based indexing)
    HANDLE thread = GetCurrentThread();
    DWORD_PTR mask = 1 << 3; // Core 3 corresponds to bit 3
    SetThreadAffinityMask(thread, mask);
    
    int n = 1000000;
    int iterations = 100;
    int sampleSize = 10;

    cout << "Preparing shared data for n = " << n << "..." << endl;

    // 2. Generate test data globally only once before all tests
    vector<vector<int>> shared_C(sampleSize, vector<int>(n));
    mt19937 rng(42); 
    for (int s = 0; s < sampleSize; ++s) {
        for (int i = 0; i < n; ++i) {
            uniform_int_distribution<int> dist(0, i);
            shared_C[s][i] = dist(rng);
        }
    }
    cout << "Data ready. Starting benchmarks." << endl << endl;

    for (int round = 1; round <= 5; round++) {
        printf("--- Round %d ---\n", round);
        
        // Use identical shared_C for comparison without initialization overhead
        runAlgorithmTest("MyrvoldRuskey", MyrvoldRuskey_unrank, shared_C, n, iterations);
        runAlgorithmTest("PositionPure", PositionPure_unrank, shared_C, n, iterations);
    }

    return 0;
}
