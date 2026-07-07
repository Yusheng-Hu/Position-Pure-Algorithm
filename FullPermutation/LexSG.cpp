/**
 * @file LexSG.cpp
 * @brief Performance-improved Permutation Generation Algorithm (Lexicographic + SG State Machine)
 * @copyright Copyright (c) 2026 [ Yusheng-Hu ]. All rights reserved.
 * @license This project is licensed under the MIT License - see the LICENSE file for details.
 *
 * Program Details:
 * - Implements three permutation generation algorithms for performance comparison:
 *   1. std::next_permutation (standard library)
 *   2. Original Algorithm L (Knuth's Algorithm L)
 *   3. LexSG (Lexicographic + State Machine hybrid)
 * - Features: High-precision timing (std::chrono), optimization-resistant checksum verification,
 *   callback-based permutation processing.
 * - Performance: Designed for minimal branch misprediction and low overhead.
 *
 * Environment:
 * - Platform: Windows / Linux
 * - Compiler: GCC/MinGW or MSVC
 */
#include <iostream>
#include <algorithm>
#include <numeric>
#include <chrono>
#include <vector>

#pragma GCC optimize ("O3")

typedef void (*PermCallback)(const int*, int);

// 1. Standard library implementation
void generate_all_std(int* arr, int n, PermCallback callback) {
    std::iota(arr, arr + n, 0);
    do {
        callback(arr, n);
    } while (std::next_permutation(arr, arr + n));
}

// 2. Original Algorithm L (Baseline)
void generate_all_original(int* arr, int n, PermCallback callback) {
    std::iota(arr, arr + n, 0);
    while (true) {
        callback(arr, n);
        int i = n - 2;
        while (i >= 0 && arr[i] >= arr[i + 1]) i--;
        if (i < 0) break;
        int j = n - 1;
        while (arr[j] <= arr[i]) j--;
        std::swap(arr[i], arr[j]);
        std::reverse(arr + i + 1, arr + n);
    }
}

// 3. LexSG pure logic (no unrolling, state machine)
void generate_all_lexsg(int* arr, int n, PermCallback callback) {
    std::iota(arr, arr + n, 0);
    std::vector<int> c(n, 0);
    callback(arr, n);
    while (true) {
        c[n - 2] = 1;
        std::swap(arr[n - 2], arr[n - 1]);
        callback(arr, n);
        
        c[n - 2] = 0;
        int i = n - 3;
        while (i >= 0 && c[i] >= (n - 1 - i)) {
            c[i] = 0;
            i--;
        }
        if (i < 0) break;
        c[i]++;
        std::swap(arr[i], arr[n - c[i]]);
        std::reverse(arr + i + 1, arr + n);
        callback(arr, n);
    }
}

// Global context
uint64_t g_count = 0, g_check = 0;
const uint64_t g_limit = ~0ULL;

void callback_func(const int* perm, int n) {
    g_count++;
    g_check += perm[n-1];
}

double run_test(const char* name, void (*func)(int*, int, PermCallback), int n, double& ns_per_perm, uint64_t& checksum) {
    int* arr = new int[n];
    g_count = 0; 
    g_check = 0;
    
    auto start = std::chrono::high_resolution_clock::now();
    func(arr, n, callback_func);
    auto end = std::chrono::high_resolution_clock::now();
    
    double elapsed_s = std::chrono::duration<double>(end - start).count();
    ns_per_perm = elapsed_s * 1e9 / g_count;
    checksum = g_check;
    
    delete[] arr;
    return elapsed_s;
}

int main() {
    const int N = 13;
    std::cout << "=== Performance Comparison (N=" << N << ") ===\n\n";

    double ns_std, ns_orig, ns_lexsg;
    uint64_t chk_std, chk_orig, chk_lexsg;
    double t_std  = run_test("std::next_permutation", generate_all_std, N, ns_std, chk_std);
    double t_orig = run_test("Original Algorithm L",  generate_all_original, N, ns_orig, chk_orig);
    double t_lexsg = run_test("LexSG Pure Logic",     generate_all_lexsg, N, ns_lexsg, chk_lexsg);
    
    std::cout << "\n| Algorithm | Time (s) | ns/perm | Speedup vs std | Checksum |\n";
    std::cout << "|---|---|---|---|---|\n";
    std::cout << "| std::next_permutation | " << t_std << " | " << ns_std << " | 1.00x | " << chk_std << " |\n";
    std::cout << "| Original Algorithm L  | " << t_orig << " | " << ns_orig << " | " << (ns_std / ns_orig) << "x | " << chk_orig << " |\n";
    std::cout << "| LexSG Pure Logic      | " << t_lexsg << " | " << ns_lexsg << " | " << (ns_std / ns_lexsg) << "x | " << chk_lexsg << " |\n";
    
    return 0;
}
