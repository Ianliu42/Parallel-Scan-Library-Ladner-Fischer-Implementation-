# Parallel Scan Library (Ladner-Fischer Implementation)


## Introduction
This project implements a **generic, high-performance Parallel Prefix Sum (Scan)** algorithm in C++17 using the **Ladner-Fischer** topology.

Parallel scan is a fundamental primitive in parallel computing and a critical building block for:
**Deep Learning:** Computing Softmax in Transformer models (LLMs).
**Stream Compaction:** Efficient data filtering in GPU kernels.
**Polynomial Evaluation** and **Parallel Sorting**.

This implementation leverages `std::async` and `std::future` to manage concurrency, providing an efficient, task-based parallel solution that scales with multi-core processors.

## Key Features
* **Generic Design:** Utilizes C++ Templates (`template <typename T>`) to support various numeric types (e.g., `int`, `float`, `double`) seamlessly.
  
* **Arbitrary Input Size:** Unlike basic implementations restricted to powers of 2, this library includes a smart **Heaper** class that automatically handles data padding and alignment, supporting datasets of any size.
  
* **Task-Based Parallelism:** Implements a hybrid parallel/serial execution strategy using `std::async`. It spawns threads for upper tree levels and switches to serial execution at deeper levels to minimize context-switching overhead.

* **Cache-Friendly Layout:**
  Uses an implicit heap structure (Array-mapped tree) to store intermediate sums, optimizing memory locality compared to pointer-based tree structures.

## Build & Usage

### Prerequisites
* **C++ Compiler:** Requires C++17 support (GCC, Clang, or MSVC).
* **CMake:** Version 3.10 or higher.

## Algorithm Details
The Ladner-Fischer algorithm executes in two passes with O(n) work and O(log n) depth:
* Up-Sweep (Reduce Phase): Builds a sum tree from the bottom up. Each internal node calculates the sum of its children. This phase is parallelized to allow concurrent summation of independent sub-blocks.
* Down-Sweep (Distribute Phase): Traverses down the tree to compute the final prefix sums. Each node passes its accumulated "incoming sum" to its children.

