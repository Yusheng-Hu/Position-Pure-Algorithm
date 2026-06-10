
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

alignas(16) uint8_t flat_lut_N5[FLAT_STEPS][XMM_LANES];

// ── 1. Precompute: SIMD blind-shuffle masks ───────────────────────────
void precompute_only_flat_lut_N5() {
    // 修复方案：将数组大小增加到 16，以满足 SIMD 优化的对齐要求，
    // 同时避免 std::next_permutation 在处理过小容器时被编译器优化导致的溢出警告。
    std::array<uint8_t, 16> P; 
    for (int i = 0; i < TAIL_DEPTH; ++i) P[i] = i;

    for (int step = 0; step < FLAT_STEPS; ++step) {
        std::array<uint8_t, 16> M;
        for (int j = 0; j < TAIL_DEPTH; ++j) M[P[j]] = j;
        
        // 使用实际有效的区间 [0, TAIL_DEPTH) 进行置换
        std::next_permutation(P.begin(), P.begin() + TAIL_DEPTH);
        
        std::memset(flat_lut_N5[step], 0, XMM_LANES);
        for (int i = 0; i < TAIL_DEPTH; ++i) {
            flat_lut_N5[step][i] = M[P[i]];
        }
    }
}

// ── 2. Accelerated engine ──────────────────────────────────────────
unsigned long long benchmark_accelerated(int N) {
    std::vector<int> D(N);
    for(int i = 0; i < N; ++i) D[i] = i;
    
    alignas(16) uint8_t buffer[32] = {0}; 
    
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

// ── 3. Main driver ───────────────────────────────────────────────────
int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    int N = std::atoi(argv[1]);
    
    precompute_only_flat_lut_N5();

    auto s2 = std::chrono::high_resolution_clock::now();
    unsigned long long c2 = benchmark_accelerated(N);
    auto e2 = std::chrono::high_resolution_clock::now();

    double d2 = std::chrono::duration<double>(e2 - s2).count();
    std::cout << N << " Acc(s): " << d2 << " | Total: " << c2 << std::endl;

    return 0;
}
