# Position Pro Algorithm

Official implementation and interactive visualizations of the **Position Pro (PP)** linear-time unranking algorithm.

## 🚀 Live Demonstrations
* **[View Position Algorithm Animation](https://yusheng-hu.github.io/Position-Pro-Algorithm/Position.html)**
* **[View Position Pro (PP) Algorithm Animation](https://yusheng-hu.github.io/Position-Pro-Algorithm/PositionPro.html)**

## 📄 Academic Status
* **Preprint**: Submitted to TechRxiv (In Moderation)
* **Status**: $O(n)$ complexity achieved, improving upon the classic Myrvold-Ruskey logic.

## 💻 Source Code
The core C++ implementations of the algorithms can be found here:
* **[map_perm_algorithms.cpp](./map_perm_algorithms.cpp)**: Includes `Position_unrank`, `Position_rank`, `PositionPro_unrank`, and `PositionPro_rank`.

## 🛠️ Usage
These algorithms achieve $O(n)$ time complexity by optimizing the indexing logic $D[i]$, providing a more efficient alternative to the classic Myrvold-Ruskey method.

## 📚 Background & Acknowledgments
The **Position Pro (PP)** algorithm is built upon the foundational work of **Wendy Myrvold and Frank Ruskey**, specifically their $O(n)$ unranking algorithm (2001). 

While the MR algorithm uses a specific swap-based approach, the PP method introduces a more intuitive indexing logic $D[i]$ that simplifies the implementation and enhances understanding of the mapping process.

## 💡 Why Position Pro?
The **Position Pro (PP)** algorithm provides a more intuitive $O(n)$ implementation compared to the classic Myrvold-Ruskey (MR) method:
* **Simpler Indexing**: Replaces the traditional swap-based unranking with a direct mapping logic.
* **Educational Value**: The provided visualizations make the complex mapping process easy to understand.
