#include <chrono>
#include <iostream>
#include <random>

#include "../src/phase_1.cpp"



int main() {
    

    const int rows = 10000, cols = 10000;

    // Allocate memory dynamically on the heap
    double* matrix = new double[rows * cols];
    double* vector = new double[cols];
    double* result = new double[rows];

    // Initialize matrix and vector
    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            matrix[i * cols + j] = static_cast<double>(rand()) / RAND_MAX;
        }
    }

    for (int j = 0; j < cols; ++j) {
        vector[j] = static_cast<double>(rand()) / RAND_MAX;
    }


    auto start = std::chrono::high_resolution_clock::now();
    multiply_mv_row_major(matrix, rows, cols, vector, result);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration =
        std::chrono::duration_cast<std::chrono::milliseconds>(end - start);
    std::cout << "Time taken for matrix-vector multiplication: "
              << duration.count() << " ms" << std::endl;

    delete[] matrix;
    delete[] vector;
    delete[] result;
    return 0;
}
