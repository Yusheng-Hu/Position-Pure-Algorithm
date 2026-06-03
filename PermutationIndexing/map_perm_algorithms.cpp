/**
 * Official Implementation of Position Method & Position Pure (PP) Algorithms
 * Author: Yusheng Hu
 * Research: A Linear-Time Generation Algorithm for Permutations
 * Repository: https://github.com/Yusheng-Hu/Position-Pure-Algorithm
 */

#include <algorithm> // for std::swap
#include <chrono>    // for performance measurement
#include <cstdlib>   // for atoi
#include <cstring>   // for memcpy
#include <fstream>   // for file I/O
#include <iomanip>   // for setprecision
#include <iostream>  // for input/output
#include <random>    // for random number generation
#include <vector>    // for vector container

using namespace std;

// Myrvold-Ruskey unrank algorithm
// Converts factorial number representation (C) to permutation (D)
void MyrvoldRuskey_unrank(const vector<int> &C, vector<int> &D) {
  int n = C.size();
  for (int i = 0; i < n; ++i) {
    D[i] = i;
  }
  // Core unranking logic: Process from right to left
  for (int i = n - 1; i >= 1; --i) {
    const int k = C[i];
    const int temp = D[i];
    D[i] = D[k];
    D[k] = temp;
  }
}

// Myrvold-Ruskey rank algorithm
// Converts permutation (D) to factorial number representation (C)
void MyrvoldRuskey_rank(const vector<int> &D, vector<int> &C) {
  int n = D.size();
  vector<int> pi = D;
  vector<int> pi_inv(n);

  // Calculate inverse permutation
  for (int i = 0; i < n; i++) {
    pi_inv[pi[i]] = i;
  }

  C.resize(n);
  int rank = 0;

  // Recursively calculate rank
  for (int k = n; k > 1; k--) {
    int s = pi[k - 1];
    C[k - 1] = s;

    // Swap operation
    swap(pi[k - 1], pi[pi_inv[k - 1]]);
    swap(pi_inv[s], pi_inv[k - 1]);
  }
  C[0] = 0;
}

// Position Mapping unrank algorithm
// Converts factorial index array C to permutation D
void Position_unrank(const vector<int> &C, vector<int> &D) {
  int n = C.size();
  vector<int> M; // Static vector to avoid reallocation on each call
  if (M.size() != n) {
    M.resize(n); // Only resize if needed
  }

  memcpy(&D[0], &C[0], n * sizeof(int));

  for (int i = 0; i < n; ++i) {
    int C_i = D[i];
    int M_C_i = M[C_i];
    M[i] = M_C_i;
    M[C_i] = i;
    D[M_C_i] = i;
  }
}

// Position Mapping rank algorithm
// Converts permutation D to factorial index array C
void Position_rank(const vector<int> &D, vector<int> &C) {
  int n = D.size();
  // C.resize(n);         // Ensure C has the correct size
  vector<int> M(n, 0); // Initialize vector with correct size
  C = D;

  // Traverse D forward to generate M array
  for (int i = 0; i < n; ++i) {
    M[C[i]] = i;
  }
  // Traverse D backward to generate C array
  for (int i = n - 1; i >= 0; --i) {
    // Add debugging
    C[M[i]] = C[i];
    M[C[i]] = M[i];
  }
}

// PositionPure unrank algorithm
void PositionPure_unrank(const vector<int> &C, vector<int> &D) {
  for (int i = 0; i < C.size(); ++i) {
    const int a = C[i];
    D[i] = D[a];
    D[a] = i;
  }
}

// PositionPure rank algorithm
void PositionPure_rank(const vector<int> &D, vector<int> &C) {
  const int n = D.size();
  static vector<int> M;
  if (M.size() < (size_t)n) M.resize(n);

  // 1. 唯一的一次全量拷贝与映射建立
  for (int i = 0; i < n; ++i) {
      const int val = D[i];
      C[i] = val;
      M[val] = i;
  }

  // 2. 纯粹的单步循环还原
  for (int i = n - 1; i >= 0; --i) {
      // 利用寄存器锁定变量，减少内存 load 次数
      const int target_pos = M[i];
      const int val_at_i = C[i];

      // 核心交换与映射维护
      C[target_pos] = val_at_i;
      M[val_at_i] = target_pos;
      
      // 产出结果
      C[i] = target_pos;
  }
}

// Original PositionPure rank algorithm (for comparison)
/*
void PositionPure_rank(const vector<int> &D, vector<int> &C) {
  int n = D.size();
  C.resize(n);

  vector<int> D_local = D;
  vector<int> M(n, 0);

  for (int i = 0; i < n; ++i) {
    M[D_local[i]] = i;
  }

  for (int i = n - 1; i >= 0; --i) {
    C[i] = M[i];
    D_local[M[i]] = D_local[i];
    M[D_local[i]] = M[i];
  }
}
*/

