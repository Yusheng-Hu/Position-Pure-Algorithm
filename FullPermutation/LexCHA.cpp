/**
 * Official Implementation of LexCHA Indexing Algorithms
 * Final Fix: Using 16-byte aligned buffer for manual permutation to 
 * satisfy GCC's aggressive optimization safety checks.
 */

#include <iostream>
#include <vector>
#include <iomanip>
#include <immintrin.h>
#include <cstring>
#include <algorithm>
#include <cstdint>
#include <chrono>

constexpr int TAIL_DEPTH = 5;
constexpr int FLAT_STEPS = 119;
constexpr int XMM_LANES = 16;

alignas(16) uint8_t flat_lut_N5[FLAT_STEPS][XMM_LANES];

// ── 1. Manual permutation with 16-byte safe buffer ───────────────────
void next_perm_manual(uint8_t* p, int n) {
    int i = n - 1;
    while (i > 0 && p[i - 1] >= p[i]) i--;
    if (i <= 0) {
        // Use a local 16-byte aligned array to satisfy the compiler's 
        // need for safety during reverse operations.
        alignas(16) uint8_t temp[16];
        for(int k=0; k<n; ++k) temp[k] = p[n-1-k];
        for(int k=0; k<n; ++k) p[k] = temp[k];
        return;
    }
    int j = n - 1;
    while (p[j] <= p[i - 1]) j--;
    std::swap(p[i - 1], p[j]);
    
    // Reverse the tail using the same safe logic
    alignas(16) uint8_t tail[16];
    int tail_len = n - i;
    for(int k=0; k<tail_len; ++k) tail[k] = p[n-1-k];
    for(int k=0; k<tail_len; ++k) p[i+k] = tail[k];
}

void precompute_only_flat_lut_N5() {
    // Declaring 16 bytes instead of 5 to align with SIMD register sizes
    alignas(16) uint8_t P[16]; 
    for (int i = 0; i < TAIL_DEPTH; ++i) P[i] = i;

    for (int step = 0; step < FLAT_STEPS; ++step) {
        uint8_t M[TAIL_DEPTH];
        for (int j = 0; j < TAIL_DEPTH; ++j) M[P[j]] = j;
        
        next_perm_manual(P, TAIL_DEPTH);
        
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

int main(int argc, char* argv[]) {
    if (argc < 2) return 1;
    int N = std::atoi(argv[1]);
    precompute_only_flat_lut_N5();
    
    auto s2 = std::chrono::high_resolution_clock::now();
    unsigned long long c2 = benchmark_accelerated(N);
    auto e2 = std::chrono::high_resolution_clock::now();

    double d2 = std::chrono::duration<double>(e2 - s2).count();
    std::cout << "N=" << N << " | Acc(s): " << d2 << " | Count: " << c2 << std::endl;
    return 0;
}
