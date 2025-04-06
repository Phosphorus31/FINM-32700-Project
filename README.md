# FINM-32700-Project

This repository hosts the course project for FINM 32700 *Advanced Computing for Finance* in which we design and implement components of a high frequency trading (HFT) system using advanced C++ techniques.

## Team Member(s)

- Kevin Wang (<kevwang@uchicago.edu>)

(Just me for now.)

## Build Instructions

TODO.

## Discussion Questions / Report Contents

> 1. Explain the key differences between pointers and references in C++. When would you choose to use a pointer over a reference, and vice versa, in the context of implementing numerical algorithms?

Pointers and references both allow indirect access to data, but pointers can be reassigned and can be null, while references are bound to a specific object and cannot be null.

In the context of numerical algorithms, pointers are preferred for dynamically allocated arrays and matrices because they offer flexibility, support for pointer arithmetic, and allow efficient in-place modification of large data structures. They also make it possible to indicate missing or optional data via `nullptr`.

References, on the other hand, are more appropriate for small, non-null inputs where clarity and safety are more important than flexibility. In this project, raw pointers are used to manipulate dynamically allocated memory directly and to avoid unnecessary copies during computation.

> 2. How does the row-major and column-major storage order of matrices affect memory access patterns and cache locality during matrix-vector and matrix-matrix multiplication? Provide specific examples from your implementations and benchmarking results.

In `multiply_mv_row_major`, since the matrix is stored in row-major order, we loop over every row and within each row, the elements are stored contiguously in memory and accessed sequentially, which gives good spatial locality.

In `multiply_mv_col_major`, where the matrix is now stored in column-major order, we now loop over every column first and use it to repeatedly multiply by the same component within the vector. We exploit the spatial locality by accessing the columns, similar as before, but we further benefit from repeatedly accessing the same vector component. Through this, we get higher cache hits and better performance.

We see in the benchmark results that `multiply_mv_col_major` typically outperforms `multiply_mv_row_major` by about a factor of 2.

| Matrix Size | `multiply_mv_row_major` (ms) | `multiply_mv_col_major` (ms) |
| ----------- | ---------------------------- | ---------------------------- |
| 256×256     | 0 ± 0                        | 0 ± 0                        |
| 1024×1024   | 3.6 ± 0.49                   | 1 ± 0                        |
| 4096×4096   | 57.5 ± 0.67                  | 25.2 ± 0.4                   |
| 16384×16384 | 924.7 ± 3.2                  | 439.3 ± 14                   |

In `multiply_mm_naive` we assume that both A and B are stored in row-major order. When we try to multiply these matrices like one does previously, every matrix element in the result is the inner product of a row of A and a column of B. While rows of A are stored contiguously in memory, accessing columns of B requires jumping through memory and has frequent cache misses.

In `multiply_mm_transposed_b` with B_tranposed in row-major order, calculating matrix elements of AB requires taking the inner product of a row of A and a row of B_transposed (column of B). Both of these are in row-major order and are stored contiguously in memory, which means that we benefit from spatial locality on both ends and much better performance than before.

Indeed, in the benchmarking results, we see that

| Matrix Size | `multiply_mm_naive` (ms) | `multiply_mm_transposed_b` (ms) |
| ----------- | ------------------------ | ------------------------------- |
| 64×64       | 0 ± 0                    | 0 ± 0                           |
| 256×256     | 56.1 ± 0.3               | 55.9 ± 0.3                      |
| 1024×1024   | 5700 ± 110               | 3714 ± 22                       |

> 3. Describe how CPU caches work (L1, L2, L3) and explain the concepts of temporal and spatial locality. How did you try to exploit these concepts in your optimizations?

Modern CPUs use a hierarchy of caches, L1, L2, and L3, to speed up memory access. The L1 cache is the smallest and fastest, located closest to the CPU core, followed by the larger but slower L2, and then the even larger and slower shared L3 cache. These caches store recently accessed data to avoid the expensive latency of fetching from main memory. Spatial locality refers to the tendency of a program to access memory locations that are close to one another, while temporal locality refers to reusing the same memory location within a short time frame.

In our optimizations, we exploited spatial locality by accessing matrix rows or columns sequentially in memory as they are stored contiguously, ensuring that when a cache line is loaded, multiple elements can be accessed before it’s evicted. For example, in `multiply_mv_row_major`, we iterate through matrix rows contiguously in memory, which fits well with row-major layout. In `multiply_mv_col_major`, we similarly iterate through rows contiguously and we also reuse the same vector component throughout the computation, exploiting temporal locality as well.

In `multiply_mm_transposed_b`, we explicitly transposed B to convert otherwise scattered column-wise access into sequential row-wise access, again boosting spatial locality and cache hit rates. These changes significantly reduced cache misses and improved performance, especially on larger matrices that stress the cache hierarchy.

> 4. What is memory alignment, and why is it important for performance? Did you observe a significant performance difference between aligned and unaligned memory in your experiments? Explain your findings.

Memory alignment refers to the practice of arranging data in memory at addresses that are multiples of a word size (e.g., 8 or 16 bytes). Proper alignment ensures that data can be accessed in the fewest possible memory operations, often allowing the CPU to use optimized instructions like Single Instruction, Multiple Data (SIMD). When data is unaligned, the CPU may need to perform extra reads, combine multiple memory accesses, or even issue penalties like pipeline stalls, all of which degrade performance.

We experimented with aligning memory and compared the performance of `multiply_mv_row_major`. Surprisingly, we found no significant difference between the two. It is likely the case that the standard `std::vector` typically provides sufficiently aligned memory for our computations. That, or I'm doing something completely wrong. I cannot say for sure. If we had to perform heavy SIMD vectorization or used non-standard data layouts, explicit alignment can yield more measurable performance improvements.

| Matrix Size | unaligned (ms) | aligned (ms) |
| ----------- | -------------- | ------------ |
| 256×256     | 0 ± 0          | 0 ± 0        |
| 1024×1024   | 3.6 ± 0.49     | 3 ± 0        |
| 4096×4096   | 57.5 ± 0.67    | 58.5 ± 1.4   |
| 16384×16384 | 924.7 ± 3.2    | 980 ± 150    |

<!-- TODO: perhaps compare other functions -->

> 5. Discuss the role of compiler optimizations (like inlining) in achieving high performance. How did the optimization level affect the performance of your baseline and optimized implementations? What are the potential drawbacks of aggressive optimization?

Compiler optimizations can play a crucial role in achieving high performance in numerical code. Optimizations such as function inlining, loop unrolling, vectorization, and dead code elimination can significantly reduce overhead and improve instruction-level parallelism. Inlining small, frequently-called functions (like our multiplication routines) removes the overhead of function calls and enables the compiler to further optimize across function boundaries, such as reordering or fusing loops.

We pass the `-O3` flag and compare to the previous performance with no compiler optimization (`-O0`).

For `multiply_mv_row_major`:

| Matrix Size | `-O0` (ms)  | `-O3` (ms)  |
| ----------- | ----------- | ----------- |
| 256×256     | 0 ± 0       | 0 ± 0       |
| 1024×1024   | 3.6 ± 0.49  | 0.1 ± 0.3   |
| 4096×4096   | 57.5 ± 0.67 | 15.2 ± 0.4  |
| 16384×16384 | 924.7 ± 3.2 | 251.6 ± 1.5 |

For `multiply_mv_col_major`:

| Matrix Size | `-O0` (ms) | `-O3` (ms) |
| ----------- | ---------- | ---------- |
| 256×256     | 0 ± 0      | 0 ± 0      |
| 1024×1024   | 1 ± 0      | 0 ± 0      |
| 4096×4096   | 25.2 ± 0.4 | 2 ± 0      |
| 16384×16384 | 439.3 ± 14 | 65.6 ± 9.7 |

Despite the heavy compiler optimizations, our more localization optimized versions still retain its advantage.

For `multiply_mm_naive`:

| Matrix Size | `-O0` (ms) | `-O3` (ms) |
| ----------- | ---------- | ---------- |
| 64×64       | 0 ± 0      | 0 ± 0      |
| 256×256     | 56.1 ± 0.3 | 15.1 ± 0.3 |
| 1024×1024   | 5700 ± 110 | 1501 ± 57  |

For `multiply_mm_transposed_b`:

| Matrix Size | `-O0` (ms) | `-O3` (ms)   |
| ----------- | ---------- | ------------ |
| 64×64       | 0 ± 0      | 0 ± 0        |
| 256×256     | 55.9 ± 0.3 | 8 ± 0        |
| 1024×1024   | 3714 ± 22  | 919.2 ± 31.6 |

Similarly, our more localization optimized versions still retain its advantage despite heavy compiler optimizations.

However, we note that aggressive optimizations can have drawbacks. They can make debugging harder, since the optimized binary may not match the original source code's flow. Some optimizations can lead to numerical instability (e.g., reordering floating-point operations), and overly aggressive inlining or unrolling might increase binary size or cause instruction cache thrashing. Despite these trade-offs, enabling high-level optimizations is essential for maximizing performance in computational kernels like ours.

> 6. Based on your profiling experience, what were the main performance bottlenecks in your initial implementations? How did your profiling results guide your optimization efforts?

From profiling different benchmarks involving `multiply_mm_naive` and `multiply_mm_transposed_b`, we can see that indeed the most time have been spent on the lines

```
sum += matrixA[i * colsA + k] * matrixB[k * colsB + j];
```

and

```
matrixA[i * colsA + k] * matrixB_transposed[j * rowsB + k];
```

respectively. This is when the computer needs to retrieve the data and is where the most benefit is derived from more frequent cache hits via optimizations in locality.

Images of the profiler output are included for reference:

![naive](/images/Screenshot%202025-04-05%20at%2023.44.21.png)

![tranposed](/images/Screenshot%202025-04-05%20at%2023.51.03.png)

> 7. Reflect on the teamwork aspect of this assignment. How did dividing the initial implementation tasks and then collaborating on analysis and optimization work? What were the challenges and benefits of this approach?

I guess this is not the most applicable since I'm working alone... In other news, I would have appreciated this project being more guided on the parts that we did not yet cover, such as memory alignment.
