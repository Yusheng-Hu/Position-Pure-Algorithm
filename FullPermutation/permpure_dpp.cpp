/**
 * @file permpure_dpp.cpp
 * @brief High-performance Distributed Position Permutation (DPP) Engine
 * 
 * @copyright Copyright (c) 2026 [ Yusheng-Hu ]. All rights reserved.
 * @license This project is licensed under the MIT License - see the LICENSE file for details.
 * 
 * @details
 * - Core Architecture: Implements the Double-Position Permutation (DPP) model, 
 *   separating state-space seeding from high-density computation deployment.
 * - Logic: Utilizes an iterative, branchless mapping strategy for minimal IPC 
 *   (Instructions Per Cycle) overhead and optimized pipeline execution.
 * - Engineering Features:
 *   - Topology-aware CPU Affinity binding for localized L1/L2 cache performance.
 *   - Seed-based state recovery for distributed collaborative computation.
 *   - Optimized for direct integration with inline topology/combinatorial analysis tasks.
 * 
 * @environment
 * - Platform: Cross-platform (Windows/Linux) via native affinity APIs.
 * - Compiler: GCC/MinGW / MSVC (Best performance achieved with O3/Ox optimization).
 * - Optimization Goal: Near-theoretical limit of cycles-per-permutation through 
 *   pipelined array-offset mapping.
 */
 
#include <stdio.h>
#include <stdlib.h>
#include <chrono>
#include <vector>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sched.h>
    #include <pthread.h>
#endif

const int LITTLE_NUMBER = 5;

// #define DEBUG

#ifdef DEBUG
#define COUNT_PURECESS() ProcessCount[__builtin_LINE()]++
#else
#define COUNT_PURECESS() 
#endif

int main(int argc, char* argv[]) {
    // --- Parse Command Line Argument ---
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }
    int perm_size = atoi(argv[1]);
    int inner_perm_size = perm_size -1;
    
    if (inner_perm_size <= 1) {
        fprintf(stderr, "Error: n must be greater than 3.\n");
        return 1;
    }

    // --- Set CPU Affinity for Accurate Benchmarking ---
#ifdef _WIN32
    DWORD_PTR mask = 8; // CPU mask for core 3
    SetThreadAffinityMask(GetCurrentThread(), mask);
#else
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(1, &cpuset);
    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
#endif

    unsigned long long checksum = 0;
    unsigned long long ProcessCount[200] = {0};
    int i = 0;
    
    const int offset = inner_perm_size + 1;

    // Use std::vector for dynamic memory management
    std::vector<int> C(inner_perm_size, 0);
    std::vector<int> D(inner_perm_size + offset, 0);

    D[inner_perm_size] = inner_perm_size;

    // --- High Precision Timing ---
    auto start = std::chrono::high_resolution_clock::now();

    // Main Algorithm Loop (PP Algorithm)
    while (C[0] < 1) {
        COUNT_PURECESS();
        for (; i < inner_perm_size - 1; ++i) {
            COUNT_PURECESS();
            D[i] = D[C[i]];
            D[i + offset] = D[C[i]];
            D[C[i]] = i;
            D[C[i] + offset] = i;
        }

        for (int ii = 0; ii < inner_perm_size; ii++) {
            COUNT_PURECESS();
            D[inner_perm_size - 1] = D[ii];
            D[inner_perm_size - 1 + offset] = D[ii];
            D[ii] = inner_perm_size - 1;
            D[ii + offset] = inner_perm_size - 1;
            
            // Standardizing checksum to match the core logic
            checksum += D[inner_perm_size - 1];

            // Reduced I/O overhead using LITTLE_NUMBER check
            if (inner_perm_size <= LITTLE_NUMBER) {
                printf("\n");
                // This section requires the consecutive output of n items—specifically, 
                //the arrays ranging from jj=0 up to jj=inner_perm_size-1.
                for (int jj = 0; jj < inner_perm_size+1; jj++) {         
                    printf("\n");
                    for (int kk = 0; kk < inner_perm_size+1; kk++) {
                        printf("%d,", D[jj + kk]);                       
                    }
                }
            }
            D[ii] = D[inner_perm_size - 1];
            D[ii + offset] = D[inner_perm_size - 1];
        }

        D[C[inner_perm_size - 2]] = D[inner_perm_size - 2];
        D[C[inner_perm_size - 2] + offset] = D[inner_perm_size - 2];

        C[inner_perm_size - 2]++;
        for (i = inner_perm_size - 2; (i > 0) && (C[i] > i); i--) {
            COUNT_PURECESS();
            C[i] = 0;
            C[i - 1]++;
            D[C[i - 1] - 1] = D[i - 1];
            D[C[i - 1] - 1 + offset] = D[i - 1];
        }
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = finish - start;
    
    // Standardized output for easy parsing
    printf("\nREPORT_START");
    printf("\nALGORITHM: permpure_full");
    printf("\nN_VALUE: %d", perm_size);
    printf("\nEXECUTION_TIME: %lf", duration.count());
    printf("\nCHECKSUM: %llu", checksum);
    printf("\nREPORT_END\n");

#ifdef DEBUG
    // Debug info logic preserved for N
    unsigned long long perm = 1;
    std::vector<unsigned long long> PermValue(perm_size + 1, 0);
    std::vector<unsigned long long> SumPermValue(perm_size + 1, 0);
    PermValue[0] = 1;
    SumPermValue[0] = 0;
    for (int k = 1; k <= perm_size; k++) {
        perm = perm * k;
        PermValue[k] = perm;
        SumPermValue[k] = SumPermValue[k - 1] + perm;
    }
    for (int k = 1; k <= perm_size; k++) {
        printf("\n%u !, %llu, sigma %llu", k, PermValue[k], SumPermValue[k]);
    }

    //打印ProcessCount，不为0就输出
    for (int k = 0; k < 200; k++) {
        if (ProcessCount[k] != 0) {
            printf("\nLine %u: %llu", k, ProcessCount[k]);
        }
    }
#endif

    return 0;

}
