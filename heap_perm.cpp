/**
 * @file heap_perm.cpp
 * @brief Heap's Permutation Generation Algorithm (Cross-Platform Implementation)
 */
#include <stdio.h>
#include <time.h>
#include <chrono> // Standard C++ timing

#ifdef __linux__
#include <sched.h> // For CPU affinity on Linux
#include <pthread.h>
#endif

#ifndef PERM_SIZE
#define PERM_SIZE 12
#endif
#define LittleNumber 5

#ifdef DEBUG
#define COUNT_PROCESS() ProcessCount[__builtin_LINE()]++
#else
#define COUNT_PROCESS() 
#endif

int main() {
    // --- CPU Affinity (Linux specific) ---
    #ifdef __linux__
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(1, &cpuset); // Bind to Core 1
    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
    #endif

    unsigned long long checksum = 0;
    unsigned long long ProcessCount[200] = {0};
    int i, j;
    int D[PERM_SIZE];
    int c[PERM_SIZE];

    for (i = 0; i < PERM_SIZE; i++) {
        D[i] = i;
        c[i] = 0;
    }

    // --- High Precision Timing (Cross-Platform) ---
    auto start = std::chrono::high_resolution_clock::now();

    // Heap's algorithm logic
#if PERM_SIZE <= LittleNumber
    printf("\n");
    for (j = 0; j < PERM_SIZE; j++) printf("%d,", D[j]);
#endif
    for (i = 0; i < PERM_SIZE; i++) checksum += D[i];

    i = 1;
    while (i < PERM_SIZE) {
        COUNT_PROCESS();
        if (c[i] < i) {
            COUNT_PROCESS();
            if (i % 2 == 0) {
                int temp = D[0]; D[0] = D[i]; D[i] = temp;
            } else {
                int temp = D[c[i]]; D[c[i]] = D[i]; D[i] = temp;
            }

#if PERM_SIZE <= LittleNumber
            printf("\n");
            for (j = 0; j < PERM_SIZE; j++) printf("%d,", D[j]);
#endif
            for (j = 0; j < PERM_SIZE; j++) checksum += D[j];

            c[i]++;
            i = 1;
        } else {
            COUNT_PROCESS();
            c[i] = 0;
            i++;
        }
    }

    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = finish - start;

    printf("\nheap_perm\t%u\t%lf seconds", PERM_SIZE, duration.count());
    printf("\nD[PERM_SIZE-1] = %d, checksum = %llu\n", D[PERM_SIZE-1], checksum);

    return 0;
}
