[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.18170157.svg)](https://doi.org/10.5281/zenodo.18170157)

# Position Pro Algorithm

Official implementation and interactive visualizations of the **Position Pro (PP)** linear-time unranking algorithm.

## 🚀 Live Demonstrations
* **[View Position Algorithm Animation](https://yusheng-hu.github.io/Position-Pro-Algorithm/Position.html)**
* **[View Position Pro (PP) Algorithm Animation](https://yusheng-hu.github.io/Position-Pro-Algorithm/PositionPro.html)**

## 📄 Academic Status
* **Preprint**: need long time.
* **Status**: $O(n)$ complexity achieved, improving upon the classic Myrvold-Ruskey logic.

## 💻 Source Code
The core C++ implementations of the algorithms can be found here:
* **[map_perm_algorithms.cpp](./map_perm_algorithms.cpp)**: Includes `Position_unrank`, `Position_rank`, `PositionPro_unrank`, and `PositionPro_rank`.
* **[permpro_full.cpp](./permpro_full.cpp)**: Serves as a high-performance reference implementation for generating all permutations of a set using the **PositionPro** iterative algorithm. 

## 🛠️ Usage
These algorithms achieve $O(n)$ time complexity by optimizing the indexing logic $D[i]$, providing a more efficient alternative to the classic Myrvold-Ruskey method.

## 📚 Background & Acknowledgments
The **Position Pro (PP)** algorithm is built upon the foundational work of **Wendy Myrvold and Frank Ruskey**, specifically their $O(n)$ unranking algorithm (2001). 

While the MR algorithm uses a specific swap-based approach, the PP method introduces a more intuitive indexing logic $D[i]$ that simplifies the implementation and enhances understanding of the mapping process.

## PositionPro: Iterative Permutation Generation

The **PositionPro** algorithm is a high-performance, iterative approach to generating all permutations of a set. By utilizing an iterative state machine rather than traditional recursion, it significantly reduces function call overhead and optimizes CPU branch prediction efficiency.

### Key Features
* **Iterative State Machine**: Eliminates recursion depth limits and stack-related performance bottlenecks.
* **Hardware Affinity Binding**: Includes native Windows API support (`SetThreadAffinityMask`) to lock execution to a specific logical core, minimizing context-switching noise and cache misses.
* **High-Precision Benchmarking**: Performance is measured using the Windows High-Precision Event Timer (`QueryPerformanceCounter`) for nanosecond-level accuracy.

### Performance Benchmarks

The following results were recorded on a single core of a mobile-class processor. Despite the modest clock speed, the algorithm maintains massive throughput, processing over a billion permutations per second.

**Test Environment:**
* **CPU**: Intel(R) Core(TM) i7-8550U @ 1.80GHz
* **Affinity**: Bound to Core 3
* **OS**: Windows 10/11
* **Compiler**: GCC/MinGW with `-O3` optimization

| $N$ (Size) | Total Permutations ($N!$) | Execution Time (Seconds) | Throughput (Perms/Sec) |
| :--- | :--- | :--- | :--- |
| **12** | 479,001,600 | **0.272373** | ~1.75 Billion |
| **13** | 6,227,020,800 | **3.696829** | **~1.68 Billion** |
| **14** | 87,178,291,200 | **48.328881** | ~1.80 Billion |

### Analysis
The results demonstrate exceptional instruction-level parallelism. On the i7-8550U platform, **PositionPro** consistently achieves a throughput of approximately **1.8 billion permutations per second**, showcasing near-linear scaling relative to the mathematical complexity $O(N!)$.

### 📖 Citation
If you use this algorithm in your research, please cite it as:
> HU, Y. (2025). Position Method: A Linear-Time Generation Algorithm for Permutations (Version v1.0.0). GitHub Repository. DOI: 10.5281/zenodo.18170157

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

## 💡 Why Position Pro?
The **Position Pro (PP)** algorithm provides a more intuitive $O(n)$ implementation compared to the classic Myrvold-Ruskey (MR) method:
* **Simpler Indexing**: Replaces the traditional swap-based unranking with a direct mapping logic.
* **Educational Value**: The provided visualizations make the complex mapping process easy to understand.

### 🚀 Evolution of Efficiency: Beyond Heap's Algorithm

The **Position-Pro Algorithm** and **Circle-Permutation-Algorithm** represent a paradigm shift in combinatorial generation by optimizing the fundamental cost of state transitions.

* **Heap's Algorithm (The Classical Baseline):** Long considered the gold standard due to its *swap-based* approach. While swaps were once thought to be the minimum-cost operation, they involve multiple memory accesses and conditional logic that limit peak performance.
* **Ives' Iterative Algorithm:** Improved upon Heap's by utilizing a *single-shift and single-assignment* pattern, reducing the overhead of element movement.
* **Position-Pro Algorithm (Our Work):** Achieves a new frontier by **eliminating conditional branching** and implementing a highly refined *shift-and-assign* strategy. 
    * **Scalability:** Optimized for large $N$.
    * **Concurrency:** Stateless design allows for massive **parallel execution**, a feat traditional recursive algorithms cannot achieve.
* **[Circle-Permutation-Algorithm](https://github.com/Yusheng-Hu/Circle-Permutation-Algorithm) (Upcoming):** Specifically engineered for circular symmetry. While it introduces controlled spatial overhead, it shatters traditional performance barriers by realizing an **O((n-1)!)** complexity framework.
