[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)
[![Language: C++](https://img.shields.io/badge/Language-C%2B%2B-red.svg)](#)
[![Complexity: O(n)](https://img.shields.io/badge/Complexity-O(n)-brightgreen.svg)](#)
[![DOI](https://zenodo.org/badge/DOI/10.5281/zenodo.18170157.svg)](https://doi.org/10.5281/zenodo.18170157)
![Permutation Generation](https://img.shields.io/badge/Field-Combinatorial_Algorithms-blue)
[![ORCID](https://img.shields.io/badge/ORCID-0009--0005--1980--5751-A6CE39?logo=orcid&logoColor=white)](https://orcid.org/0009-0005-1980-5751)
---

## 📢 Important Update: From "Position Pro" to "Position Pure"

**Please note:** Starting from version **v1.1.1**, this algorithm has been officially renamed from **Position Pro** to **Position Pure (PP)**.

### Why the name change?
1. **Trademark Consideration:** We found that "Position Pro" is a registered trademark in several industrial and commercial sectors. To ensure the algorithm's academic independence and avoid legal confusion, we have transitioned to a unique name.
2. **Technical Essence (Why "Pure"?):** The name **"Pure"** more accurately describes the algorithm's technical implementation. In computer science, "pure" often implies a clean, branchless execution flow. 
3. **Core Feature - Branchless Logic:** Unlike the base version, **Position Pure (PP)** achieves its linear-time performance through direct element-wise shifting and replacement. **It eliminates conditional branching (no `if` statements in the core loop)**, leading to a "purer" and faster execution path on modern CPUs.

---

# Position Pure Algorithm

Official implementation and interactive visualizations of the **Position Pure (PP)** linear-time unranking algorithm.

## 🚀 Live Demonstrations
* **[View Position Algorithm Animation](https://yusheng-hu.github.io/Position-Pure-Algorithm/Position.html)**
* **[View Position Pure (PP) Algorithm Animation](https://yusheng-hu.github.io/Position-Pure-Algorithm/PositionPure.html)**

## 📄 Academic Status
* **Preprint**: need long time.
* **Status**: $O(n)$ complexity achieved, improving upon the classic Myrvold-Ruskey logic.

## 💻 Source Code
The core C++ implementations of the algorithms can be found here:
* **[map_perm_algorithms.cpp](./map_perm_algorithms.cpp)**: Includes `Position_unrank`, `Position_rank`, `PositionPure_unrank`, and `PositionPure_rank`.
* **[permPure_full.cpp](./permPure_full.cpp)**: Serves as a high-performance reference implementation for generating all permutations of a set using the **PositionPure** iterative algorithm. 

## 🛠️ Usage
These algorithms achieve $O(n)$ time complexity by optimizing the indexing logic $D[i]$, providing a more efficient alternative to the classic Myrvold-Ruskey method.

## 📚 Background & Acknowledgments
The **Position Pure (PP)** algorithm is built upon the foundational work of **Wendy Myrvold and Frank Ruskey**, specifically their $O(n)$ unranking algorithm (2001). 

While the MR algorithm uses a specific swap-based approach, the PP method introduces a more intuitive indexing logic $D[i]$ that simplifies the implementation and enhances understanding of the mapping process.

## PositionPure: Iterative Permutation Generation

The **PositionPure** algorithm is a high-performance, iterative approach to generating all permutations of a set. By utilizing an iterative state machine rather than traditional recursion, it significantly reduces function call overhead and optimizes CPU branch prediction efficiency.

### Key Features
* **Iterative State Machine**: Eliminates recursion depth limits and stack-related performance bottlenecks.
* **Hardware Affinity Binding**: Includes native Windows API support (`SetThreadAffinityMask`) to lock execution to a specific logical core, minimizing context-switching noise and cache misses.
* **High-Precision Benchmarking**: Performance is measured using the Windows High-Precision Event Timer (`QueryPerformanceCounter`) for nanosecond-level accuracy.
* 

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

### Performance Benchmarks: Full Permutation Generation

The table below demonstrates the performance of **PositionPure (permPure_full)** compared to the classic **Heap's Algorithm (heap_perm)** across sizes $n=11, 12, 13$.

| Size ($n$) | Algorithm | Test 1 (s) | Test 2 (s) | Test 3 (s) | **Average (s)** | **Speedup** |
| :--- | :--- | :--- | :--- | :--- | :--- | :--- |
| **11** | **permPure_full** | 0.025236 | 0.026826 | 0.023158 | **0.025073** | **5.27x** |
| | heap_perm | 0.140333 | 0.127427 | 0.128642 | 0.132134 | 1.00x |
| **12** | **permPure_full** | 0.272237 | 0.270233 | 0.291104 | **0.277858** | **6.18x** |
| | heap_perm | 1.607632 | 1.645012 | 1.899579 | 1.717408 | 1.00x |
| **13** | **permPure_full** | 4.043442 | 3.877965 | 3.960660 | **3.960689** | **5.94x** |
| | heap_perm | 23.504669 | 23.567322 | 23.554655 | 23.542215 | 1.00x |

---

#### **Key Insights**
* **Significant Speedup:** `permPure_full` consistently outperforms Heap's Algorithm by a factor of approximately **6x**.
* **High Throughput:** For $n=13$, `permPure_full` processes over **6.2 billion permutations** in under 4 seconds, showcasing exceptional instruction-level efficiency.
* **Algorithmic Efficiency:** The performance gap highlights the superior memory access patterns and lower computational overhead inherent in the PositionPure algorithm.

---

#### **Performance Insight**
* **Consistent Efficiency:** `permPure_full` consistently outperforms Heap's Algorithm by a factor of **~6x**.
* **Scalability:** As the permutation space grows factorially, the performance gap remains stable, demonstrating excellent algorithmic efficiency for large-scale generation.
* **Optimized Throughput:** The data indicates that `permPure_full` handles over **2 billion permutations per second** on the tested hardware, significantly reducing the computational overhead for exhaustive search tasks.

### Analysis
The results demonstrate exceptional instruction-level parallelism. On the i7-8550U platform, **PositionPure** consistently achieves a throughput of approximately **1.8 billion permutations per second**, showcasing near-linear scaling relative to the mathematical complexity $O(N!)$.

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

### 🚀 Evolution of Efficiency: Beyond Heap's Algorithm

The **Position-Pure Algorithm** and **Circle-Permutation-Algorithm** represent a paradigm shift in combinatorial generation by optimizing the fundamental cost of state transitions.

* **Heap's Algorithm (The Classical Baseline):** Long considered the gold standard due to its *swap-based* approach. While swaps were once thought to be the minimum-cost operation, they involve multiple memory accesses and conditional logic that limit peak performance.
* **Ives' Iterative Algorithm:** Improved upon Heap's by utilizing a *single-shift and single-assignment* pattern, reducing the overhead of element movement.
* **Position-Pure Algorithm (Our Work):** Achieves a new frontier by **eliminating conditional branching** and implementing a highly refined *shift-and-assign* strategy. 
    * **Scalability:** Optimized for large $N$.
    * **Concurrency:** Stateless design allows for massive **parallel execution**, a feat traditional recursive algorithms cannot achieve.
* **[Circle-Permutation-Algorithm](https://github.com/Yusheng-Hu/Circle-Permutation-Algorithm) (Upcoming):** Specifically engineered for circular symmetry. While it introduces controlled spatial overhead, it shatters traditional performance barriers by realizing an **O((n-1)!)** complexity framework.

# PositionPure: Exploring Analytical Permutations

> **"Full permutation generation is a well-studied field. However, providing efficient, random access to specific elements within massive permutations remains a compelling challenge—one where we are only just beginning to explore."**

Traditional approaches, such as the classic Myrvold-Ruskey algorithm, typically rely on sequential swapping processes. While highly efficient for full generation, these state-dependent models often require the entire permutation to reside in memory, which can be a constraint for large-scale distributed tasks.

**PositionPure (PP)** explores a different path by treating permutations as an **analytical mapping**. This project introduces an alternative framework designed to decouple individual elements from the sequential dependency chain, facilitating new approaches to large-scale computation:

### 💡 Core Concepts

* **On-Demand "Lazy" Access:** Determine the value at any given index $k$ without the need to allocate or generate the full array. This "zero-memory" approach offers a practical way to handle $N > 10^9$ where memory overhead becomes a bottleneck.
* **Independent Parallelism:** Because each position can be traced independently, computation can be distributed across multiple cores or nodes with minimal inter-thread communication.
* **Analytical Lookup:** By shifting from process simulation to mapping analysis, the algorithm provides forward and backward tracking capabilities that remain stable even as $N$ grows beyond traditional memory limits.

---

### 🔍 Architectural Comparison

| Feature | Sequential Models (e.g., Myrvold-Ruskey) | PositionPure (Analytical Approach) |
| :--- | :--- | :--- |
| **Methodology** | Sequential Swapping | Path-independent Mapping |
| **Access Pattern** | Full generation before access | On-demand random access |
| **Scalability** | Optimized for single-node throughput | Optimized for distributed/decoupled tasks |
| **Resource Focus** | Performance through state-density | Flexibility through analytical lookup |
