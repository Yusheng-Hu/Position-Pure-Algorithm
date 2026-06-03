#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <random>
#include <chrono>
#include <string>
#include <iomanip>
#include <cassert>
#include "map_perm_algorithms.h"

using namespace std;

#ifdef _WIN32
#include <windows.h>
#else
#include <pthread.h>
#include <sched.h>
#include <unistd.h>
#endif

void lock_to_single_core() {
#ifdef _WIN32
    DWORD_PTR mask = 1;
    SetThreadAffinityMask(GetCurrentThread(), mask);
#else
    cpu_set_t cpuset;
    CPU_ZERO(&cpuset);
    CPU_SET(0, &cpuset);
    pthread_setaffinity_np(pthread_self(), sizeof(cpu_set_t), &cpuset);
#endif
}

void MR_rank(const vector<int> &D, vector<int> &C) {
    MyrvoldRuskey_rank(D, C);
}

void MR_unrank(const vector<int> &C, vector<int> &D) {
    MyrvoldRuskey_unrank(C, D);
}

bool verify_logic(int n) {
    vector<int> original(n), C(n), restored(n);
    iota(original.begin(), original.end(), 0);
    mt19937 g(42);
    
    vector<string> cases = {"Sorted", "Reverse", "Random"};
    for (const auto& name : cases) {
        if (name == "Reverse") reverse(original.begin(), original.end());
        if (name == "Random") shuffle(original.begin(), original.end(), g);

        PositionPure_rank(original, C);
        PositionPure_unrank(C, restored);
        for (int i = 0; i < n; ++i) {
            if (original[i] != restored[i]) return false;
        }

        MR_rank(original, C);
        MR_unrank(C, restored);
        for (int i = 0; i < n; ++i) {
            if (original[i] != restored[i]) return false;
        }
    }
    return true;
}

typedef void (*RankFunc)(const vector<int>&, vector<int>&);

double measure_speed(RankFunc func, int n, int its, const string& dist) {
    vector<int> D(n), C(n);
    iota(D.begin(), D.end(), 0);
    if (dist == "Reverse") reverse(D.begin(), D.end());
    if (dist == "Random") {
        mt19937 g(123);
        shuffle(D.begin(), D.end(), g);
    }

    func(D, C);

    auto start = chrono::high_resolution_clock::now();
    for (int i = 0; i < its; ++i) {
        func(D, C);
    }
    auto end = chrono::high_resolution_clock::now();
    
    chrono::duration<double, nano> diff = end - start;
    return diff.count() / its;
}

int main() {
    lock_to_single_core();

    cout << "====================================================" << endl;
    cout << "PHASE 1: CORRECTNESS VALIDATION" << endl;
    cout << "====================================================" << endl;
    
    if (verify_logic(10000)) {
        cout << "[SUCCESS] Both PP and MR algorithms passed round-trip tests." << endl;
    } else {
        cerr << "[FAILED] Logic error detected in algorithms!" << endl;
        return 1;
    }

    cout << "\n==================================================================" << endl;
    cout << "PHASE 2: INDUSTRIAL BENCHMARK (Core-Locked Recommended)" << endl;
    cout << "==================================================================" << endl;
    cout << left << setw(10) << "N" << setw(12) << "Dist" 
         << setw(15) << "MR (ns/it)" << setw(15) << "PP (ns/it)" << "Speedup" << endl;
    cout << "------------------------------------------------------------------" << endl;

    vector<int> sizes = {1000000, 10000000, 100000000};
    vector<string> dists = {"Random", "Sorted", "Reverse"};

    for (int n : sizes) {
        int its = (n <= 1000) ? 10000 : (n <= 100000 ? 5 : 1);
        
        for (const auto& d : dists) {
            double t_mr = measure_speed(MR_unrank, n, its, d);
            double t_pp = measure_speed(PositionPure_unrank, n, its, d);
            double speedup = t_mr / t_pp;

            cout << left << setw(10) << n 
                 << setw(12) << d 
                 << setw(15) << fixed << setprecision(1) << t_mr
                 << setw(15) << fixed << setprecision(1) << t_pp
                 << setprecision(2) << speedup << "x" << endl;
        }
        cout << "------------------------------------------------------------------" << endl;
    }

    return 0;
}
