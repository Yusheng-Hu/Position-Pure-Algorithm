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

// ── 2. Accelerated Engine (With Integrated Vector Checksum) ─────────
unsigned long long benchmark_accelerated(int N, unsigned long long& acc_checksum) {
    std::vector<uint8_t> D(N);
    for(int i = 0; i < N; ++i) D[i] = static_cast<uint8_t>(i);
    
    alignas(16) uint8_t buffer[16] = {0}; 
    std::memcpy(buffer, &D[N - TAIL_DEPTH], TAIL_DEPTH);
    __m128i p_reg = _mm_load_si128((__m128i*)buffer);

    unsigned long long total_count = 1;
    unsigned long long max_perms = 1;
    for(int i = 1; i <= N; ++i) max_perms *= i;

    // Initialize checksum with initial state
    acc_checksum = 0;
    for (int i = 0; i < N; ++i) {
        acc_checksum += D[i] * (i + 1);
    }

    // Precompute weight vector for tail elements (positions N-4 .. N)
    __m128i w_words = _mm_setr_epi16(
        (short)(N - 4), (short)(N - 3), (short)(N - 2), (short)(N - 1), (short)N,
        0, 0, 0
    );

    while (total_count < max_perms) {
        // Prefix hash: flat during 119 steps, compute once
        unsigned long long prefix_sum = 0;
        for (int i = 0; i < N - TAIL_DEPTH; ++i) {
            prefix_sum += D[i] * (i + 1);
        }

        // SIMD tail checksum accumulator
        __m128i tail_acc = _mm_setzero_si128();

        for (int step = 0; step < FLAT_STEPS; ++step) {
            __m128i mask = _mm_load_si128((__m128i*)flat_lut_N5[step]);
            p_reg = _mm_shuffle_epi8(p_reg, mask);

            // Vectorized dot product: unpack u8->u16, multiply by weights, accumulate
            __m128i p_words = _mm_unpacklo_epi8(p_reg, _mm_setzero_si128());
            __m128i prod = _mm_mullo_epi16(p_words, w_words);
            tail_acc = _mm_add_epi16(tail_acc, prod);
        }
        total_count += FLAT_STEPS;

        // Gather accumulated tail checksum
        alignas(16) int16_t tail_sums[8];
        _mm_store_si128((__m128i*)tail_sums, tail_acc);
        unsigned long long step_tail_checksum = tail_sums[0] + tail_sums[1] + tail_sums[2] + tail_sums[3] + tail_sums[4];

        // Merge prefix and tail
        acc_checksum += (prefix_sum * FLAT_STEPS) + step_tail_checksum;

        // Write back and advance by next_permutation
        _mm_store_si128((__m128i*)buffer, p_reg);
        std::memcpy(&D[N - TAIL_DEPTH], buffer, TAIL_DEPTH);
        
        if (std::next_permutation(D.begin(), D.end())) {
            total_count++;
            
            // Scalar checksum for fallback state
            unsigned long long fallback_checksum = 0;
            for (int i = 0; i < N; ++i) {
                fallback_checksum += D[i] * (i + 1);
            }
            acc_checksum += fallback_checksum;

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

    // Benchmark 1: Standard Method + Checksum
    std::vector<uint8_t> V(N);
    for(int i = 0; i < N; ++i) V[i] = static_cast<uint8_t>(i);

    unsigned long long c1 = 0, std_checksum = 0;

    auto s1 = std::chrono::high_resolution_clock::now();
    do {
        c1++;
        unsigned long long local_hash = 0;
        for (int i = 0; i < N; ++i) {
            local_hash += V[i] * (i + 1);
        }
        std_checksum += local_hash;
    } while (std::next_permutation(V.begin(), V.end()));
    auto e1 = std::chrono::high_resolution_clock::now();
    
    double d1 = std::chrono::duration<double>(e1 - s1).count();
    if (d1 < 1e-9) d1 = 1e-9; 

    // Benchmark 2: Accelerated Method + Checksum
    unsigned long long acc_checksum = 0;

    auto s2 = std::chrono::high_resolution_clock::now();
    unsigned long long c2 = benchmark_accelerated(N, acc_checksum);
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
    if (std_checksum != acc_checksum) {
        std::cerr << "Error: Checksum mismatch! Std: " << std_checksum << " Acc: " << acc_checksum << std::endl;
        return 1;
    }

    return 0;
}
