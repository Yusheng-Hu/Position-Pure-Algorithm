# Position Pure Algorithm

[![GitHub stars](https://img.shields.io/github/stars/Yusheng-Hu/Position-Pure-Algorithm?style=social)](https://github.com/Yusheng-Hu/Position-Pure-Algorithm)
![Permutation Generation](https://img.shields.io/badge/Field-Combinatorial_Algorithms-blue)
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-red.svg)](#)
[![Complexity: O(n)](https://img.shields.io/badge/Complexity-O(n)-brightgreen.svg)](#)

[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.18184520.svg)](https://doi.org/10.5281/zenodo.18184520)
[![SSRN](https://img.shields.io/badge/SSRN-6285581-blue)](https://ssrn.com/abstract=6285581)
[![ORCID](https://img.shields.io/badge/ORCID-0009--0005--1980--5751-A6CE39?logo=orcid&logoColor=white)](https://orcid.org/0009-0005-1980-5751)
---

# Position Pure Algorithm

Official implementation and interactive visualizations of the **Position Pure (PP)** linear-time ranking/unranking algorithm.

### 🚀 Evolution of Efficiency: Beyond Heap's Algorithm

The **Position-Pure Algorithm** and **Ring-Cascade-Permutation-Algorithm** represent a paradigm shift in combinatorial generation by optimizing the fundamental cost of state transitions.

* **Heap's Algorithm (The Classical Baseline):** Long considered the gold standard due to its *swap-based* approach. While swaps were once thought to be the minimum-cost operation, they involve multiple memory accesses and conditional logic that limit peak performance.
* **Ives' Iterative Algorithm:** Improved upon Heap's by utilizing a *single-shift and single-assignment* pattern, reducing the overhead of element movement.
* **Position-Pure Algorithm (Our Work):** Achieves a new frontier by **eliminating conditional branching** and implementing a highly refined *shift-and-assign* strategy. 
    * **Scalability:** Optimized for large $N$.
    * **Concurrency:** Stateless design allows for massive **parallel execution**, a feat traditional recursive algorithms cannot achieve.
* **[Ring-Cascade-Permutation-Algorithm](https://github.com/Yusheng-Hu/Ring-Cascade-Permutation-Algorithm) :** Specifically engineered for circular symmetry. While it introduces controlled spatial overhead, it shatters traditional performance barriers by realizing an **O((n-1)!)** complexity framework.

#### **Key Insights**
* **Significant Speedup:** `permPure_full` consistently outperforms Heap's Algorithm by a factor of approximately **7x**.
* **Algorithmic Efficiency:** The performance gap highlights the superior memory access patterns and lower computational overhead inherent in the PositionPure algorithm.
* **Scalability:** As the permutation space grows factorially, the performance gap remains stable, demonstrating excellent algorithmic efficiency for large-scale generation.

---


## 🚀 Live Demonstrations
* **[View Position Algorithm Animation](https://yusheng-hu.github.io/Position-Pure-Algorithm/Position.html)**
* **[View Position Pure (PP) Algorithm Animation](https://yusheng-hu.github.io/Position-Pure-Algorithm/PositionPure.html)**


## 🚀 Full Permutation generation: Iterative Permutation Generation

The **PositionPure** algorithm is a high-performance, iterative approach to generating all permutations of a set. By utilizing an iterative state machine rather than traditional recursion, it significantly reduces function call overhead and optimizes CPU branch prediction efficiency.

The algorithm utilizes an **iterative state machine** to eliminate recursion overhead and leverages **hardware affinity binding** via the Windows API to minimize context-switching noise, with performance verified to nanosecond-level accuracy through **high-precision benchmarking**.

[//]: # (UNIQUE_PP_ALGO_BENCHMARK_AMD_START)

**Last Run:** Fri May 08 15:40:29 2026 UTC / Fri May 08 23:40:29 2026 (UTC+8)
**Environment:** AMD EPYC 7763 64-Core Processor (GitHub Actions Runner)

| N | Heap (s) | PP (s) | Speedup |
|---|---|---|---|
| 9 | 0.005753 | 0.000632 | 9.10x |
| 10 | 0.059474 | 0.006454 | 9.21x |
| 11 | 0.648535 | 0.064187 | 10.10x |
| 12 | 7.634664 | 0.707448 | 10.79x |
| 13 | 101.130642 | 8.450858 | 11.96x |

[//]: # (UNIQUE_PP_ALGO_BENCHMARK_AMD_END)

[//]: # (UNIQUE_PP_ALGO_BENCHMARK_INTEL_START)

**Last Run:** Fri Feb 06 07:20:50 2026 UTC / Fri Feb 06 15:20:50 2026 (UTC+8)
**Environment:** Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz (GitHub Actions Runner)

| N | Heap (s) | PP (s) | Speedup |
|---|---|---|---|
| 9 | 0.006039 | 0.000871 | 6.93x |
| 10 | 0.059160 | 0.007174 | 8.24x |
| 11 | 0.657323 | 0.075407 | 8.71x |
| 12 | 7.809986 | 1.004821 | 7.77x |
| 13 | 103.446021 | 11.693761 | 8.84x |
| 14 | 1468.660328 | 162.986940 | 9.01x |

[//]: # (UNIQUE_PP_ALGO_BENCHMARK_INTEL_END)

---

## 🚀 Comparison with Python's Built-in itertools

At the request of Reddit users and other community members, a performance comparison with Python’s built-in `itertools` was conducted. Results are based on the implementation in `Position-Pure-Algorithm/python/pp_iter.py`.

* **Runtime Environment**: The tests were performed using **PyPy3**. Since the standard `itertools` library is C-optimized, using PyPy3 helps bridge the low-level language gap, allowing for a more authentic comparison of algorithmic efficiency.
* **Results**: Benchmark tests show that this algorithm improves performance by at least **1.4x**, with potential gains reaching over **2x** compared to the standard library.
* **Future Plans**: For applications requiring even higher performance, a **C-compiled version** should be considered.

### 🐍 Position Pure Iterator Performance (PyPy)

[//]: # (PYTHON_PP_ITER_BENCHMARK_AMD_START)

**Last Run:** Mon May 11 11:41:27 2026 UTC / Mon May 11 19:41:27 2026 (UTC+8)
**Environment:** AMD EPYC 9V74 80-Core Processor (GitHub Actions Runner)

| N | Total Permutations | Itertools (s) | Position Pure (s) | Speed-up |
| :--- | :--- | :--- | :--- | :--- |
| 10 | 3,628,800 | 0.0862s | 0.0537s | **1.60x** |
| 11 | 39,916,800 | 0.9966s | 0.4571s | **2.18x** |
| 12 | 479,001,600 | 12.4014s | 5.2005s | **2.38x** |
| 13 | 6,227,020,800 | 168.1315s | 69.1447s | **2.43x** |
| 14 | 87,178,291,200 | 2370.9107s | 1252.8360s | **1.89x** |

[//]: # (PYTHON_PP_ITER_BENCHMARK_AMD_END)

[//]: # (PYTHON_PP_ITER_BENCHMARK_INTEL_START)

**Last Run:** Mon Mar 02 02:33:27 2026 UTC / Mon Mar 02 10:33:27 2026 (UTC+8)
**Environment:** Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz (GitHub Actions Runner)

| N | Total Permutations | Itertools (s) | Position Pure (s) | Speed-up |
| :--- | :--- | :--- | :--- | :--- |
| 10 | 3,628,800 | 0.1143s | 0.0764s | **1.50x** |
| 11 | 39,916,800 | 1.1867s | 0.5373s | **2.21x** |
| 12 | 479,001,600 | 14.3020s | 5.8797s | **2.43x** |
| 13 | 6,227,020,800 | 191.1971s | 74.4842s | **2.57x** |
| 14 | 87,178,291,200 | 2890.1775s | 1108.1700s | **2.61x** |

[//]: # (PYTHON_PP_ITER_BENCHMARK_INTEL_END)

### ⚡ Dual-Core Parallel Performance (GitHub Actions)

[//]: # (DUAL_CORE_BENCHMARK_START)

**Last Run:** Mon Mar  9 12:47:21 UTC 2026 / Mon Mar 09 20:47:21 2026 (UTC+8)
**Environment:** AMD EPYC 7763 64-Core Processor                 (GitHub Actions Runner)

| N | Core ID | Execution Time | Throughput (G/s) | Total Count |
|---|---|---|---|---|
| 10 | Core 0 | 0.0030s | 0.61 | - |
| 10 | Core 1 | 0.0030s | 0.60 | 3628800 |
| 11 | Core 0 | 0.0314s | 0.64 | - |
| 11 | Core 1 | 0.0308s | 0.65 | 39916800 |
| 12 | Core 0 | 0.3639s | 0.66 | - |
| 12 | Core 1 | 0.3596s | 0.67 | 479001600 |
| 13 | Core 0 | 4.5475s | 0.68 | - |
| 13 | Core 1 | 4.5354s | 0.69 | 6227020800 |
| 14 | Core 0 | 62.4459s | 0.70 | - |
| 14 | Core 1 | 61.9731s | 0.70 | 87178291200 |

[//]: # (DUAL_CORE_BENCHMARK_END)


## 🚀 Liner rank unrank: Position Pure (PP) vs. Myrvold-Ruskey (MR) 

[//]: # (INDUSTRIAL_PERFORMANCE_BENCHMARK_DATA_SECTION_AMD_START)

**Last Run:** Wed Jun 03 07:58:19 2026 UTC / Wed Jun 03 15:58:19 2026 (UTC+8)
**Environment:** AMD EPYC 9V74 80-Core Processor (GitHub Actions Runner)

| N | Dist | MR (ns/it) | PP (ns/it) | Speedup |
|---|---|---|---|---|
| 1000000 | Random | 1294495.0 | 1110278.0 | 1.17x |
| 1000000 | Sorted | 751148.0 | 774974.0 | 0.97x |
| 1000000 | Reverse | 819210.0 | 840513.0 | 0.97x |
| 10000000 | Random | 58314758.0 | 49926817.0 | 1.17x |
| 10000000 | Sorted | 8411906.0 | 7711254.0 | 1.09x |
| 10000000 | Reverse | 8867089.0 | 8364575.0 | 1.06x |
| 100000000 | Random | 837091710.0 | 825949195.0 | 1.01x |
| 100000000 | Sorted | 86981169.0 | 77215596.0 | 1.13x |
| 100000000 | Reverse | 91475973.0 | 81683489.0 | 1.12x |

[//]: # (INDUSTRIAL_PERFORMANCE_BENCHMARK_DATA_SECTION_AMD_END)


[//]: # (INDUSTRIAL_PERFORMANCE_BENCHMARK_DATA_SECTION_INTEL_START)

**Last Run:** Sat Feb 21 13:32:53 2026 UTC / Sat Feb 21 21:32:53 2026 (UTC+8)
**Environment:** Intel(R) Xeon(R) Platinum 8370C CPU @ 2.80GHz (GitHub Actions Runner)

| N | Dist | MR (ns/it) | PP (ns/it) | Speedup |
|---|---|---|---|---|
| 1000 | Random | 697.2 | 578.0 | 1.21x |
| 1000 | Sorted | 651.1 | 476.6 | 1.37x |
| 1000 | Reverse | 666.3 | 574.2 | 1.16x |
| 100000 | Random | 109696.3 | 104611.9 | 1.05x |
| 100000 | Sorted | 63265.3 | 46876.3 | 1.35x |
| 100000 | Reverse | 69900.0 | 79963.0 | 0.87x |
| 1000000 | Random | 2331791.2 | 2143208.0 | 1.09x |
| 1000000 | Sorted | 712214.9 | 470972.9 | 1.51x |
| 1000000 | Reverse | 853533.5 | 870776.9 | 0.98x |

[//]: # (INDUSTRIAL_PERFORMANCE_BENCHMARK_DATA_SECTION_INTEL_END)

### Dual Position Pure (DPP) Algorithm

**DPP (Dual Position Pure)** is a high-performance algorithm for full permutation generation, designed for modern processor architectures. By utilizing a **dual-ring topology** and **spatial decoupling**, it shifts the generation paradigm from local swaps to structured construction, compressing control overhead to an $(n-1)$ dimensional subspace.

#### Performance Benchmarks
The following table shows the execution time (in seconds) for generating all permutations of order $N$:

| $N$ (Order) | Execution Time (s) |
| :---: | :---: |
| 9 | 0.000142 |
| 10 | 0.001212 |
| 11 | 0.011577 |
| 12 | 0.123500 |
| 13 | 1.431980 |

*Note: Benchmarks conducted on standard GitHub-hosted runners with `-O3` optimization.*

---
## LexCHA (Lexicographical Co-designed Hardware Acceleration) Performance Benchmarks

The following tables show the performance comparison between the standard C++ library (`std::next_permutation`) and the LexCHA SIMD-accelerated engine across different CPU architectures.

### Environment 1: Cloud VM (GitHub Actions / AMD EPYC)
* **Compiler:** `g++ -O3 -march=native -std=c++17`
* **Target:** Strict byte-level vectorization (`_mm_shuffle_epi8`)

| N | Std (s) | Acc (s) | Std (ns/perm) | Acc (ns/perm) | Speedup |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **10** | 0.011107 | 0.002524 | 3.060918 | 0.695410 | **4.40x** |
| **11** | 0.118945 | 0.027397 | 2.979828 | 0.686353 | **4.34x** |
| **12** | 1.422392 | 0.329176 | 2.969494 | 0.687213 | **4.32x** |
| **13** | 18.484087 | 4.282182 | 2.968368 | 0.687677 | **4.32x** |

### Environment 2: Local Host (Intel Core Architecture)
* **Compiler:** `g++ -O3 -march=native -std=c++17`
* **Advantage:** Dedicated Intel shuffle ports and optimal Store-to-Load Forwarding (STLF).

| N | Std (s) | Acc (s) | Std (ns/perm) | Acc (ns/perm) | Speedup |
| :---: | :---: | :---: | :---: | :---: | :---: |
| **10** | 0.013980 | 0.001881 | 3.852431 | 0.518463 | **7.43x** |
| **11** | 0.150596 | 0.021415 | 3.772737 | 0.536486 | **7.03x** |
| **12** | 1.783026 | 0.255463 | 3.722380 | 0.533324 | **6.98x** |
| **13** | 22.455285 | 3.312170 | 3.606104 | 0.531903 | **6.78x** |

---

## 💻 Source Code
The core C++ implementations of the algorithms can be found here:
* **[map_perm_algorithms.cpp](./map_perm_algorithms.cpp)**: Includes `Position_unrank`, `Position_rank`, `PositionPure_unrank`, and `PositionPure_rank`.
* **[permPure_full.cpp](./FullPermutation/permPure_full.cpp)**: Serves as a high-performance reference implementation for generating all permutations of a set using the **PositionPure** iterative algorithm. 
## 🛠 Compilation Commands
The following commands compile both algorithms using the same aggressive optimization flags to ensure a fair performance comparison:
```bash
# Compile options
g++ -O3 -std=c++11 -march=native -ffast-math -fomit-frame-pointer
```
---

## 📖 Citation

If you use this algorithm or implementation in your research, please cite it as follows:

### APA Style
> HU, Y. (2026). Position Method: A Linear-Time Generation Algorithm for Permutations (Version v1.0.0). GitHub Repository. DOI: [10.5281/zenodo.18170157](https://doi.org/10.5281/zenodo.18170157)

### BibTeX
```bibtex
@software{hu_yusheng_2026_18170157,
  author       = {Hu, Yusheng},
  title        = {Position Method: A Linear-Time Generation Algorithm for Permutations},
  year         = 2026,
  publisher    = {Zenodo},
  version      = {v1.0.0},
  doi          = {10.5281/zenodo.18170157},
  url          = {[https://doi.org/10.5281/zenodo.18170157](https://doi.org/10.5281/zenodo.18170157)}
}

```

```
Hu, Yusheng, Position Method: A Linear-Time Generation Algorithm for Permutations (February 22, 2026). Available at SSRN:
https://ssrn.com/abstract=6285581 or http://dx.doi.org/10.2139/ssrn.6285581
```

## 💡 Future Ideas & Extensions

### 1. Zero-Memory "Lazy" Permutation
Since we can determine any value at position `k` in O(N) time without generating the full array, we can implement a **Lazy-Loaded Permutation Object**. 
- **Use Case:** Accessing elements of a massive permutation (N > 10^6) that exceeds RAM capacity.
- **Concept:** Overload `operator[]` to call `PP_get_value_at_position(k)` on the fly.

### 2. SIMD-Accelerated Partial Search
The backward-tracing logic `if (C[i] == current_target_pos)` is highly vectorizable. 
- **Idea:** Use AVX-512 to scan 16 elements of `C` simultaneously.
- **Goal:** Achieve near O(1) perceived latency for individual element lookups.

### 3. Distributed Constrained Search
Leverage the independence of the `get_value` function to split a single permutation validation task across multiple CPU cores or network nodes without shared state.

## 💡 Why Position Pure?
The **Position Pure (PP)** algorithm provides a more intuitive $O(n)$ implementation compared to the classic Myrvold-Ruskey (MR) method:
* **Simpler Indexing**: Replaces the traditional swap-based unranking with a direct mapping logic.
* **Educational Value**: The provided visualizations make the complex mapping process easy to understand.

---

## 📄 Academic Status
* **Preprint**: need long time.
* **Status**: $O(n)$ complexity achieved, improving upon the classic Myrvold-Ruskey logic.

