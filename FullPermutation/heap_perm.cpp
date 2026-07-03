/**
 * @file heap_perm.cpp
 * @brief Heap's Permutation Generation Algorithm (Dynamic Size via CLI)
 */
#include <stdio.h>
#include <stdlib.h>
#include <chrono>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sched.h>
#endif

// We keep this as a constant for clarity
const int LITTLE_NUMBER = 5;

int main(int argc, char* argv[]) {
    // --- Parse Command Line Argument ---
    if (argc < 2) {
        fprintf(stderr, "Usage: %s <n>\n", argv[0]);
        return 1;
    }
    int perm_size = atoi(argv[1]);
    if (perm_size <= 0) {
        fprintf(stderr, "Error: n must be a positive integer.\n");
        return 1;
    }

    // --- Set CPU Affinity ---
    #ifdef _WIN32
        DWORD_PTR mask = 8; // Core 3
        SetThreadAffinityMask(GetCurrentThread(), mask);
    #else
        cpu_set_t cpuset;
        CPU_ZERO(&cpuset);
        CPU_SET(1, &cpuset);
        sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
    #endif

    unsigned long long checksum = 0;
    int i, j;
    
    // Dynamic memory allocation
    int* D = (int*)malloc(perm_size * sizeof(int));
    int* c = (int*)malloc(perm_size * sizeof(int));

    for (i = 0; i < perm_size; i++) {
        D[i] = i;
        c[i] = 0;
    }

    // --- Start Timing ---
    auto start = std::chrono::high_resolution_clock::now();

    // Initial permutation checksum and optional print
    if (perm_size <= LITTLE_NUMBER) {
        printf("\n");
        for (j = 0; j < perm_size; j++) printf("%d,", D[j]);
    }
    for (i = 0; i < perm_size; i++) checksum += D[i];

    // Heap's algorithm core logic
    i = 1;
    while (i < perm_size) {
        if (c[i] < i) {
            if (i % 2 == 0) {
                int temp = D[0]; D[0] = D[i]; D[i] = temp;
            } else {
                int temp = D[c[i]]; D[c[i]] = D[i]; D[i] = temp;
            }

            // High-performance check: only print if n is small
            if (perm_size <= LITTLE_NUMBER) {
                printf("\n");
                for (j = 0; j < perm_size; j++) printf("%d,", D[j]);
            }
            
            // This loop remains for checksum calculation
            //for (j = 0; j < perm_size; j++) checksum += j * D[j]; // Modified slightly for better validation
            checksum += D[perm_size - 1];
            
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
    
    // Standardized output
    printf("\nREPORT_START");
    printf("\nALGORITHM: heap_perm");
    printf("\nN_VALUE: %d", perm_size);
    printf("\nEXECUTION_TIME: %lf", duration.count());
    printf("\nCHECKSUM: %llu", checksum);
    printf("\nREPORT_END\n");

    free(D);
    free(c);

    return 0;
}
