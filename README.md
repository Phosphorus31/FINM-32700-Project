# FINM-32700-Project

This repository hosts the course project for FINM 32700 *Advanced Computing for Finance* in which we design and implement components of a high frequency trading (HFT) system using advanced C++ techniques.

## Team Member(s)

- Kevin Wang (<kevwang@uchicago.edu>)

(Just me for now.)

## Build Instructions

TODO.

## Discussion Questions

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

TODO.
