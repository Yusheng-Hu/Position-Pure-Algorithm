[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-red.svg)](#)
[![Complexity: O(n)](https://img.shields.io/badge/Complexity-O(n)-brightgreen.svg)](#)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.18184520.svg)](https://doi.org/10.5281/zenodo.18184520)
![Permutation Generation](https://img.shields.io/badge/Field-Combinatorial_Algorithms-blue)
[![ORCID](https://img.shields.io/badge/ORCID-0009--0005--1980--5751-A6CE39?logo=orcid&logoColor=white)](https://orcid.org/0009-0005-1980-5751)
[![GitHub stars](https://img.shields.io/github/stars/Yusheng-Hu/Position-Pure-Algorithm?style=social)](https://github.com/Yusheng-Hu/Position-Pure-Algorithm)

---

# Position Pure Algorithm

Official implementation and interactive visualizations of the **Position Pure (PP)** linear-time ranking/unranking algorithm.

### 🚀 Evolution of Efficiency: Beyond Heap's Algorithm

The **Position-Pure Algorithm** and **Circle-Permutation-Algorithm** represent a paradigm shift in combinatorial generation by optimizing the fundamental cost of state transitions.

* **Heap's Algorithm (The Classical Baseline):** Long considered the gold standard due to its *swap-based* approach. While swaps were once thought to be the minimum-cost operation, they involve multiple memory accesses and conditional logic that limit peak performance.
* **Ives' Iterative Algorithm:** Improved upon Heap's by utilizing a *single-shift and single-assignment* pattern, reducing the overhead of element movement.
* **Position-Pure Algorithm (Our Work):** Achieves a new frontier by **eliminating conditional branching** and implementing a highly refined *shift-and-assign* strategy. 
    * **Scalability:** Optimized for large $N$.
    * **Concurrency:** Stateless design allows for massive **parallel execution**, a feat traditional recursive algorithms cannot achieve.
* **[Circle-Permutation-Algorithm](https://github.com/Yusheng-Hu/Circle-Permutation-Algorithm) :** Specifically engineered for circular symmetry. While it introduces controlled spatial overhead, it shatters traditional performance barriers by realizing an **O((n-1)!)** complexity framework.

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

**Last Run:** Sun Mar 01 11:38:59 2026 UTC / Sun Mar 01 19:38:59 2026 (UTC+8)
**Environment:** AMD EPYC 7763 64-Core Processor (GitHub Actions Runner)

| N | Heap (s) | PP (s) | Speedup |
|---|---|---|---|
| 9 | 0.005759 | 0.000609 | 9.45x |
| 10 | 0.058161 | 0.006082 | 9.56x |
| 11 | 0.644743 | 0.063894 | 10.09x |
| 12 | 7.839334 | 0.721098 | 10.87x |
| 13 | 103.668299 | 8.718333 | 11.89x |

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

**Last Run:** Thu Feb 26 14:44:51 2026 UTC / Thu Feb 26 22:44:51 2026 (UTC+8)
**Environment:** AMD EPYC 7763 64-Core Processor (GitHub Actions Runner)

| N | Total Permutations | Itertools (s) | Position Pure (s) | Speed-up |
| :--- | :--- | :--- | :--- | :--- |
| 10 | 3,628,800 | 0.1070s | 0.0667s | **1.61x** |
| 11 | 39,916,800 | 1.2080s | 0.5684s | **2.13x** |
| 12 | 479,001,600 | 14.8491s | 6.4569s | **2.30x** |
| 13 | 6,227,020,800 | 197.0861s | 82.7780s | **2.38x** |
| 14 | 87,178,291,200 | 2853.0846s | 1155.4054s | **2.47x** |

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

**Last Run:** Tue Mar 03 02:26:07 2026 UTC / Tue Mar 03 10:26:07 2026 (UTC+8)
**Environment:** AMD EPYC 7763 64-Core Processor (GitHub Actions Runner)

| N | Dist | MR (ns/it) | PP (ns/it) | Speedup |
|---|---|---|---|---|
| 1000 | Random | 748.3 | 631.9 | 1.18x |
| 1000 | Sorted | 675.9 | 628.8 | 1.07x |
| 1000 | Reverse | 737.3 | 629.0 | 1.17x |
| 100000 | Random | 109688.1 | 88781.2 | 1.24x |
| 100000 | Sorted | 67456.5 | 64397.4 | 1.05x |
| 100000 | Reverse | 80114.6 | 67142.6 | 1.19x |
| 1000000 | Random | 1537319.6 | 1215701.2 | 1.26x |
| 1000000 | Sorted | 699238.8 | 626820.5 | 1.12x |
| 1000000 | Reverse | 795836.1 | 666185.1 | 1.19x |

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

---


## 📢 Important Update: From "Position Pro" to "Position Pure"

**Please note:** Starting from version **v1.1.1**, this algorithm has been officially renamed from **Position Pro** to **Position Pure (PP)**.

### Why the name change?
1. **Trademark Consideration:** We found that "Position Pro" is a registered trademark in several industrial and commercial sectors. To ensure the algorithm's academic independence and avoid legal confusion, we have transitioned to a unique name.
2. **Technical Essence (Why "Pure"?):** The name **"Pure"** more accurately describes the algorithm's technical implementation. In computer science, "pure" often implies a clean, branchless execution flow. 
3. **Core Feature - Branchless Logic:** Unlike the base version, **Position Pure (PP)** achieves its linear-time performance through direct element-wise shifting and replacement. **It eliminates conditional branching (no `if` statements in the core loop)**, leading to a "purer" and faster execution path on modern CPUs.
