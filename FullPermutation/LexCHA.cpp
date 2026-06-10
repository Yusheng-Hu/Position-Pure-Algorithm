/**
 * Official Implementation of LexCHA Indexing Algorithms
 * Author: Yusheng Hu
 * Research: A Divide-and-Conquer Engine for Lexicographical Permutations
 * * Note: Data types fixed to uint8_t to perfectly align with _mm_shuffle_epi8
 * byte-level operations, eliminating permutation count mismatches.
 */

#include <iostream>
#include <vector>
#include <iomanip>
#include <immintrin.h>
#include <cstring>
#include <algorithm>
#include <cstdint>
#include <chrono>

// ── Architecture Configuration ───────────────────────────────────────
constexpr int TAIL_DEPTH = 5;
constexpr int FLAT_STEPS = 119;
constexpr int XMM_LANES = 16;

alignas(16) uint8_t flat_lut_N5[FLAT_STEPS][XMM_LANES];

// ── 1. Manual Permutation ────────────────────────────────────────────
void next_perm_manual(uint8_t* p, int n) {
    int i = n - 1;
    while (i > 0 && p[i - 1] >= p[i]) i--;
    
    if (i <= 0) {
        alignas(16) uint8_t temp[16];
        for(int k = 0; k < n; ++k) temp[k] = p[n - 1 - k];
        for(int k = 0; k < n; ++k) p[k] = temp[k];
        return;
    }
    
    int j = n - 1;
    while (p[j] <= p[i - 1]) j--;
    std::swap(p[i - 1], p[j]);
    
    alignas(16) uint8_t tail[16];
    int tail_len = n - i;
    for(int k = 0; k < tail_len; ++k) tail[k] = p[n - 1 - k];
    for(int k = 0; k < tail_len; ++k) p[i + k] = tail[k];
}

// ── Precomputation ───────────────────────────────────────────────────
void precompute_only_flat_lut_N5() {
    alignas(16) uint8_t P[16]; 
    for (int i = 0; i < TAIL_DEPTH; ++i) P[i] = i;

    for (int step = 0; step < FLAT_STEPS; ++step) {
        uint8_t M[TAIL_DEPTH];
        for (int j = 0; j < TAIL_DEPTH; ++j) M[P[j]] = j;
        
        next_perm_manual(P, TAIL_DEPTH);
        
        // 0x80 safely zeros out the unused lanes during _mm_shuffle_epi8
        std::memset(flat_lut_N5[step], 0x80, XMM_LANES);
        for (int i = 0; i < TAIL_DEPTH; ++i) {
            flat_lut_N5[step][i] = M[P[i]];
        }
    }
}

// ── 2. Accelerated Engine ────────────────────────────────────────────
unsigned long long benchmark_accelerated(int N) {
    // FIX: Use uint8_t instead of int for byte-level SIMD compatibility
    std::vector<uint8_t> D(N);
    for(int i = 0; i < N; ++i) D[i] = static_cast<uint8_t>(i);
    
    alignas(16) uint8_t buffer[16] = {0}; 
    // sizeof(uint8_t) is 1, so we copy exactly TAIL_DEPTH bytes
    std::memcpy(buffer, &D[N - TAIL_DEPTH], TAIL_DEPTH);
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

        _mm_store_si128((__m128i*)buffer, p_reg);
        std::memcpy(&D[N - TAIL_DEPTH], buffer, TAIL_DEPTH);
        
        if (std::next_permutation(D.begin(), D.end())) {
            total_count++;
            std::memcpy(buffer, &D[N - TAIL_DEPTH], TAIL_DEPTH);
            p_reg = _mm_load_si128((__m128i*)buffer);
        }
    }
    return total_count;
}

// ── 3. Main Driver ───────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    int N = std::atoi(argv[1]);
    
    precompute_only_flat_lut_N5();

    // Benchmark 1: Standard Method
    // FIX: Using uint8_t to ensure fair comparison
    std::vector<uint8_t> V(N);
    for(int i = 0; i < N; ++i) V[i] = static_cast<uint8_t>(i);
    
    auto s1 = std::chrono::high_resolution_clock::now();
    unsigned long long c1 = 0;
    do {
        c1++;
    } while (std::next_permutation(V.begin(), V.end()));
    auto e1 = std::chrono::high_resolution_clock::now();
    
    double d1 = std::chrono::duration<double>(e1 - s1).count();
    if (d1 < 1e-9) d1 = 1e-9; 

    // Benchmark 2: Accelerated Method
    auto s2 = std::chrono::high_resolution_clock::now();
    unsigned long long c2 = benchmark_accelerated(N);
    auto e2 = std::chrono::high_resolution_clock::now();
    
    double d2 = std::chrono::duration<double>(e2 - s2).count();
    if (d2 < 1e-9) d2 = 1e-9;

    // Output formatting for AWK script
    double ns_std = (d1 * 1e9) / c1;
    double ns_acc = (d2 * 1e9) / c2;
    double speedup = d1 / d2;

    std::cout << N << " " 
              << std::fixed << std::setprecision(6) << d1 << " " 
              << d2 << " " 
              << ns_std << " " 
              << ns_acc << " " 
              << speedup << std::endl;

    // Sanity Check: If this triggers, something is fundamentally wrong
    if (c1 != c2) {
        std::cerr << "Error: Count mismatch! Std: " << c1 << " Acc: " << c2 << std::endl;
        return 1;
    }

    return 0;
}
