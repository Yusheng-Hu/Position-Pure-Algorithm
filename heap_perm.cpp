/**
 * @file heap_perm.cpp
 * @brief Heap's Permutation Generation Algorithm (Cross-Platform)
 */
#include <stdio.h>
#include <chrono> // Standard C++ high-precision timing

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sched.h> // Linux CPU affinity
#endif

#ifndef PERM_SIZE
#define PERM_SIZE 12
#endif
#define LittleNumber 5

int main() {
    // --- Set CPU Affinity ---
    #ifdef _WIN32
        DWORD_PTR mask = 8; // Core 3
        SetThreadAffinityMask(GetCurrentThread(), mask);
    #else
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(1, &cpuset); // Bind to Core 1 on Linux
        sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
    #endif

    unsigned long long checksum = 0;
    int i, j;
    int D[PERM_SIZE];
    int c[PERM_SIZE];

    for (i = 0; i < PERM_SIZE; i++) {
        D[i] = i;
        c[i] = 0;
    }

    // --- Start Timing ---
    auto start = std::chrono::high_resolution_clock::now();

    // Heap's algorithm logic
    #if PERM_SIZE <= LittleNumber
        printf("\n");
        for (j = 0; j < PERM_SIZE; j++) printf("%d,", D[j]);
    #endif
    for (i = 0; i < PERM_SIZE; i++) checksum += D[i];

    i = 1;
    while (i < PERM_SIZE) {
        if (c[i] < i) {
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
            c[i] = 0;
            i++;
        }
    }

    // --- End Timing ---
    auto finish = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = finish - start;

    printf("\nheap_perm\tN=%u\tTime: %lf seconds", PERM_SIZE, duration.count());
    printf("\nFinal D[%d] = %d, Checksum = %llu\n", PERM_SIZE-1, D[PERM_SIZE-1], checksum);

    return 0;
}
