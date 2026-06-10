
/**
 * Official Implementation of LexCHA Indexing Algorithms
 * Author: Yusheng Hu
 * Research: A Divide-and-Conquer Engine for Lexicographical Permutations: 
 Accelerating State Evolution via Hybrid Software-Hardware CPU Instructions
 * Repository: https://github.com/Yusheng-Hu/Position-Pure-Algorithm
 */

#include <iostream>
#include <algorithm>
#include <chrono>
#include <vector>
#include <iomanip>
#include <immintrin.h>
#include <array>
#include <cstring> 

// ── Architecture Configuration ───────────────────────────────────────
constexpr int TAIL_DEPTH = 5;
constexpr int FLAT_STEPS = 119;
constexpr int XMM_LANES = 16;

// Align to 16 bytes for SIMD operations
alignas(16) uint8_t flat_lut_N5[FLAT_STEPS][XMM_LANES];

// ── 1. Precompute: SIMD blind-shuffle masks ───────────────────────────
void precompute_only_flat_lut_N5() {
    std::array<uint8_t, TAIL_DEPTH> P;
    for (int i = 0; i < TAIL_DEPTH; ++i) P[i] = i;

    for (int step = 0; step < FLAT_STEPS; ++step) {
        std::array<uint8_t, TAIL_DEPTH> M;
        for (int j = 0; j < TAIL_DEPTH; ++j) M[P[j]] = j;
        
        std::next_permutation(P.begin(), P.end());
        
        std::memset(flat_lut_N5[step], 0, XMM_LANES);
        for (int i = 0; i < TAIL_DEPTH; ++i) {
            flat_lut_N5[step][i] = M[P[i]];
        }
    }
}

// ── 2. Accelerated engine: SIMD blind ops + next_permutation boundary skip ──
unsigned long long benchmark_accelerated(int N) {
    std::vector<int> D(N);
    for(int i = 0; i < N; ++i) D[i] = i;
    
    // Increased buffer size to 32 bytes to safely hold 20 bytes (5 ints) 
    // while maintaining 16-byte alignment for SIMD operations.
    alignas(16) uint8_t buffer[32] = {0}; 
    
    // Copy the last TAIL_DEPTH elements safely
    std::memcpy(buffer, &D[N - TAIL_DEPTH], TAIL_DEPTH * sizeof(int));
    __m128i p_reg = _mm_load_si128((__m128i*)buffer);

    unsigned long long total_count = 1;
    unsigned long long max_perms = 1;
    for(int i = 1; i <= N; ++i) max_perms *= i;

    while (total_count < max_perms) {
        for (int step = 0; step < FLAT_STEPS; ++step) {
            __m128i mask = _mm_load_si128((__m128i*)flat_lut_N5[step]);
            p_reg = _mm_shuffle_epi8(p_reg, mask);
        }
        total_count += FLAT_STEPS;

        // Store back to aligned buffer
        _mm_store_si128((__m128i*)buffer, p_reg);
        std::memcpy(&D[N - TAIL_DEPTH], buffer, TAIL_DEPTH * sizeof(int));
        
        if (std::next_permutation(D.begin(), D.end())) {
            total_count++;
            std::memcpy(buffer, &D[N - TAIL_DEPTH], TAIL_DEPTH * sizeof(int));
            p_reg = _mm_load_si128((__m128i*)buffer);
        }
    }
    return total_count;
}

// ── 3. Baseline benchmark ────────────────────────────────────────────
unsigned long long benchmark_std(int N) {
    std::vector<int> P(N);
    for (int i = 0; i < N; ++i) P[i] = i;
    unsigned long long count = 1;
    while (std::next_permutation(P.begin(), P.end())) count++;
    return count;
}

// ── 4. Main driver ───────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    int N = std::atoi(argv[1]);
    
    precompute_only_flat_lut_N5();

    auto s1 = std::chrono::high_resolution_clock::now();
    unsigned long long c1 = benchmark_std(N);
    auto e1 = std::chrono::high_resolution_clock::now();

    auto s2 = std::chrono::high_resolution_clock::now();
    unsigned long long c2 = benchmark_accelerated(N);
    auto e2 = std::chrono::high_resolution_clock::now();

    double d1 = std::chrono::duration<double>(e1 - s1).count();
    double d2 = std::chrono::duration<double>(e2 - s2).count();

    // Output formatted row: N, Std(s), Acc(s), Std_ns/perm, Acc_ns/perm, Speedup
    std::cout << N << " " << d1 << " " << d2 << " " 
              << (d1 * 1e9) / c1 << " " << (d2 * 1e9) / c2 << " " 
              << d1/d2 << "x" << std::endl;

    return 0;
}
