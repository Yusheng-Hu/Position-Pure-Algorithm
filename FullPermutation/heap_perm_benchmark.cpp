/**
 * @file heap_perm_benchmark.cpp
 * @brief Heap vs HeapOpt vs HeapSG vs Position Pure — Compile-time N benchmark
 *
 * N is a compile-time constexpr. Change N below and recompile
 * to test different sizes.
 */
#include <stdio.h>
#include <string.h>
#include <algorithm>
#include <chrono>

#ifdef _WIN32
    #include <windows.h>
#else
    #include <sched.h>
#endif

/* ========================================================================
 * Configuration
 * ======================================================================== */
#ifndef PERM_N
#define PERM_N 13
#endif
#define N PERM_N

/* Verification: define VERIFY=1 to run correctness checks before timing.
 * Default off — enable only when needed (small N). */
#ifndef VERIFY
#define VERIFY 0
#endif

/* ========================================================================
 * Output macro
 * ======================================================================== */
/* volatile read only — prevents barrel-shift elimination without unfair cost */
#define OUTPUT_PERM(ptr) do { volatile int _v = (ptr)[0]; (void)_v; } while(0)

/* ========================================================================
 * Permutation Generators
 * ======================================================================== */

/* --------------------------------------------------------------------
 * Heap's Algorithm  —  standard (baseline)
 * -------------------------------------------------------------------- */
static void run_heap(int* a, int* c) {
    for (int i = 0; i < N; i++) a[i] = i;
    memset(c, 0, N * sizeof(int));

    OUTPUT_PERM(a);

    int i = 1;
    while (i < N) {
        if (c[i] < i) {
            if (i & 1)
                std::swap(a[c[i]], a[i]);
            else
                std::swap(a[0],    a[i]);
            OUTPUT_PERM(a);
            c[i]++;
            i = 1;
        } else {
            c[i] = 0;
            i++;
        }
    }
}

/* --------------------------------------------------------------------
 * Heap's Algorithm  —  optimized (i=1 unrolled)
 * -------------------------------------------------------------------- */
static void run_heap_opt(int* a, int* c) {
    for (int i = 0; i < N; i++) a[i] = i;
    memset(c, 0, N * sizeof(int));

    OUTPUT_PERM(a);                     /* identity */

    if constexpr (N >= 2) {
        std::swap(a[0], a[1]);
        OUTPUT_PERM(a);                 /* identity's twin */

        int i = 2;
        while (i < N) {
            if (c[i] < i) {
                if (i & 1)
                    std::swap(a[c[i]], a[i]);
                else
                    std::swap(a[0],    a[i]);
                OUTPUT_PERM(a);         /* permutation #1 (high swap) */

                c[i]++;

                std::swap(a[0], a[1]);
                OUTPUT_PERM(a);         /* twin permutation */
                i = 2;
            } else {
                c[i] = 0;
                i++;
            }
        }
    }
}

/* --------------------------------------------------------------------
 * HeapSG  —  Heap Structure Generation
 * -------------------------------------------------------------------- */
static void run_heapsg(int* D, int* C) {
    for (int i = 0; i < N; i++) D[i] = i;
    memset(C, 0, N * sizeof(int));
    OUTPUT_PERM(D);

    while (true) {
        std::swap(D[0], D[1]);
        OUTPUT_PERM(D);

        C[1]++;
        int i = 1;
        for (; i < N - 1 && C[i] > i + 1; i++) {
            C[i] = 0;
            C[i + 1]++;
        }
        if (i >= N - 1) break;

        if ((i & 1) == 0)
            std::swap(D[C[i] - 1], D[i + 1]);
        else
            std::swap(D[0], D[i + 1]);
        OUTPUT_PERM(D);
    }
}

/* --------------------------------------------------------------------
 * Position Pure  —  batch-output variant
 * -------------------------------------------------------------------- */
static void run_position_pure(int* D, int* C) {
    for (int i = 0; i < N; i++) D[i] = i;
    memset(C, 0, N * sizeof(int));

    int i = 0;
    while (C[0] < 1) {
        for (; i < N - 1; ++i) {
            D[i] = D[C[i]];
            D[C[i]] = i;
        }

        for (int ii = 0; ii < N; ii++) {
            D[N - 1] = D[ii];
            D[ii] = N - 1;
            { volatile int _v = D[N - 1]; (void)_v; }  /* D[N-1] changes every iteration — prevents hoisting */
            D[ii] = D[N - 1];
        }

        D[C[N - 2]] = D[N - 2];
        C[N - 2]++;
        for (i = N - 2; (i > 0) && (C[i] > i); i--) {
            C[i] = 0;
            C[i - 1]++;
            D[C[i - 1] - 1] = D[i - 1];
        }
    }
}

/* ========================================================================
 * Benchmark Infrastructure
 * ======================================================================== */

static unsigned long long factorial(int n) {
    unsigned long long f = 1;
    for (int i = 2; i <= n; i++) f *= i;
    return f;
}

struct BenchmarkResult {
    const char*         algo_name;
    int                 n;
    unsigned long long  total_perms;
    double              time_sec;
    double              perms_per_sec;
    unsigned long long  checksum;
};

static void print_header(void) {
    printf("Comparison: Heap vs HeapOpt vs HeapSG vs Position Pure (N=%d)\n", N);
    printf("----------------------------------------------------\n");
    printf("%-10s  %5s  %15s  %18s  %15s  %20s\n",
           "Algorithm", "n",  "time (s)",    "perms / sec",
           "n! (total)", "checksum");
    printf("%-10s  %5s  %15s  %18s  %15s  %20s\n",
           "----------", "-----", "---------------", "------------------",
           "---------------", "--------------------");
}

static void print_result(const BenchmarkResult& r) {
    char perms_str[64];
    if (r.perms_per_sec > 1e9)
        sprintf(perms_str, "%.2e", r.perms_per_sec);
    else
        sprintf(perms_str, "%.0f", r.perms_per_sec);

    printf("%-10s  %5d  %15.9f  %18s  %15llu  %20llu\n",
           r.algo_name, r.n, r.time_sec, perms_str,
           r.total_perms, r.checksum);
}

/* ========================================================================
 * Verification (correctness, not timed)
 * ======================================================================== */

static unsigned long long run_heap_verify(int* a, int* c) {
    for (int i = 0; i < N; i++) a[i] = i;
    memset(c, 0, N * sizeof(int));

    unsigned long long checksum = a[0];

    if constexpr (N >= 2) {
        std::swap(a[0], a[1]);
        checksum += a[0];

        int i = 2;
        while (i < N) {
            if (c[i] < i) {
                if (i & 1)
                    std::swap(a[c[i]], a[i]);
                else
                    std::swap(a[0],    a[i]);
                checksum += a[0];

                c[i]++;

                std::swap(a[0], a[1]);
                checksum += a[0];

                i = 2;
            } else {
                c[i] = 0;
                i++;
            }
        }
    }

    return checksum;
}

static unsigned long long run_heapsg_verify(int* D, int* C) {
    for (int i = 0; i < N; i++) D[i] = i;
    memset(C, 0, N * sizeof(int));

    unsigned long long checksum = D[0];

    while (true) {
        std::swap(D[0], D[1]);
        checksum += D[0];

        C[1]++;
        int i = 1;
        for (; i < N - 1 && C[i] > i + 1; i++) {
            C[i] = 0;
            C[i + 1]++;
        }
        if (i >= N - 1) break;

        if ((i & 1) == 0)
            std::swap(D[C[i] - 1], D[i + 1]);
        else
            std::swap(D[0], D[i + 1]);
        checksum += D[0];
    }

    return checksum;
}

static unsigned long long run_position_pure_verify(int* D, int* C) {
    for (int i = 0; i < N; i++) D[i] = i;
    memset(C, 0, N * sizeof(int));

    unsigned long long checksum = 0;

    int i = 0;
    while (C[0] < 1) {
        for (; i < N - 1; ++i) {
            D[i] = D[C[i]];
            D[C[i]] = i;
        }

        for (int ii = 0; ii < N; ii++) {
            D[N - 1] = D[ii];
            D[ii] = N - 1;
            checksum += D[N - 1];
            D[ii] = D[N - 1];
        }

        D[C[N - 2]] = D[N - 2];
        C[N - 2]++;
        for (i = N - 2; (i > 0) && (C[i] > i); i--) {
            C[i] = 0;
            C[i - 1]++;
            D[C[i - 1] - 1] = D[i - 1];
        }
    }

    return checksum;
}

/* ========================================================================
 * Main
 * ======================================================================== */
int main() {
    int heap_base_a[N], heap_base_c[N];
    int heap_opt_a[N], heap_opt_c[N];
    int heapsg_D[N], heapsg_C[N];
    int pp_D[N], pp_C[N];

#ifdef _WIN32
    SetThreadAffinityMask(GetCurrentThread(), 8);
#else
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(1, &cpuset);
    sched_setaffinity(0, sizeof(cpu_set_t), &cpuset);
#endif

    unsigned long long total_perms = factorial(N);

#if VERIFY
    /* --- Verification pass (correctness, not timed) --- */
    unsigned long long heap_verify_chk  = run_heap_verify(heap_base_a, heap_base_c);
    unsigned long long heapsg_verify_chk = run_heapsg_verify(heapsg_D, heapsg_C);
    unsigned long long pp_verify_chk     = run_position_pure_verify(pp_D, pp_C);
#else
    unsigned long long heap_verify_chk  = 0;
    unsigned long long heapsg_verify_chk = 0;
    unsigned long long pp_verify_chk     = 0;
#endif

    /* --- Measurement pass (timed, no checksum) --- */
    auto t0 = std::chrono::high_resolution_clock::now();
    run_heap(heap_base_a, heap_base_c);
    auto t1 = std::chrono::high_resolution_clock::now();

    run_heap_opt(heap_opt_a, heap_opt_c);
    auto t2 = std::chrono::high_resolution_clock::now();

    run_heapsg(heapsg_D, heapsg_C);
    auto t3 = std::chrono::high_resolution_clock::now();

    run_position_pure(pp_D, pp_C);
    auto t4 = std::chrono::high_resolution_clock::now();

    double heap_base_sec = std::chrono::duration<double>(t1 - t0).count();
    double heap_opt_sec  = std::chrono::duration<double>(t2 - t1).count();
    double heapsg_sec    = std::chrono::duration<double>(t3 - t2).count();
    double pp_sec        = std::chrono::duration<double>(t4 - t3).count();

    BenchmarkResult heap_base_result = {
        "Heap", N, total_perms, heap_base_sec,
        total_perms / heap_base_sec, heap_verify_chk
    };
    BenchmarkResult heap_opt_result = {
        "HeapOpt", N, total_perms, heap_opt_sec,
        total_perms / heap_opt_sec, heap_verify_chk
    };
    BenchmarkResult heapsg_result = {
        "HeapSG", N, total_perms, heapsg_sec,
        total_perms / heapsg_sec, heapsg_verify_chk
    };
    BenchmarkResult pp_result = {
        "PositionPure", N, total_perms, pp_sec,
        total_perms / pp_sec, pp_verify_chk
    };

    print_header();
    print_result(heap_base_result);
    print_result(heap_opt_result);
    print_result(heapsg_result);
    print_result(pp_result);
    printf("----------------------------------------------------\n");
    printf("Speedup vs Heap (baseline):\n");
    printf("  HeapOpt:      %.6f\n", heap_base_sec / heap_opt_sec);
    printf("  HeapSG:       %.6f\n", heap_base_sec / heapsg_sec);
    printf("  PositionPure: %.6f\n", heap_base_sec / pp_sec);
#if VERIFY
    printf("Checksums: Heap(verify)=%llu  HeapSG(verify)=%llu  PositionPure(verify)=%llu\n",
           heap_verify_chk, heapsg_verify_chk, pp_verify_chk);
#endif
    printf("----------------------------------------------------\n");

    return 0;
}
