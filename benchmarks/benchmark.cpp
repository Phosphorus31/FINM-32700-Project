#include <chrono>
#include <iostream>
#include <random>

#include "../src/phase_1.cpp"

std::pair<double, double> benchmark_function(std::function<void()> func,
                                             int runs = 10) {
    if (runs <= 0) {
        throw std::invalid_argument("Number of runs must be positive");
    }

    std::vector<double> times(runs);

    for (int i = 0; i < runs; ++i) {
        auto start = std::chrono::high_resolution_clock::now();
        func();
        auto end = std::chrono::high_resolution_clock::now();
        times[i] =
            std::chrono::duration_cast<std::chrono::milliseconds>(end - start)
                .count();
    }

    double sum = std::accumulate(times.begin(), times.end(), 0.0);
    double mean = sum / runs;

    double sum_squares =
        std::inner_product(times.begin(), times.end(), times.begin(), 0.0);
    double stddev = std::sqrt(sum_squares / runs - mean * mean);

    return {mean, stddev};
}

void fill_random(std::vector<double>& vec) {
    for (auto& val : vec) {
        val = static_cast<double>(rand()) / RAND_MAX;
    }
}

int main() {
    std::vector<int> sizes = {256, 1024, 4096, 16384};
    const int runs = 10;

    std::cout << "Benchmarking multiply_mv_row_major:\n";

    for (int size : sizes) {
        std::vector<double> matrix(size * size);
        std::vector<double> vector_in(size);
        std::vector<double> result(size);

        fill_random(matrix);
        fill_random(vector_in);

        auto [avg, stdev] = benchmark_function(
            [&]() {
                multiply_mv_row_major(matrix.data(), size, size,
                                      vector_in.data(), result.data());
            },
            runs);

        std::cout << "Size: " << size << "x" << size << " | Avg: " << avg
                  << " ms"
                  << " | StdDev: " << stdev << " ms\n";
    }

    std::cout << std::endl;

    std::cout << "Benchmarking multiply_mv_col_major:\n";

    for (int size : sizes) {
        std::vector<double> matrix(size * size);
        std::vector<double> vector_in(size);
        std::vector<double> result(size);

        fill_random(matrix);
        fill_random(vector_in);

        auto [avg, stdev] = benchmark_function(
            [&]() {
                multiply_mv_col_major(matrix.data(), size, size,
                                      vector_in.data(), result.data());
            },
            runs);

        std::cout << "Size: " << size << "x" << size << " | Avg: " << avg
                  << " ms"
                  << " | StdDev: " << stdev << " ms\n";
    }

    std::cout << std::endl;

    sizes = {64, 256, 1024};

    std::cout << "Benchmarking multiply_mm_naive:\n";

    for (int size : sizes) {
        std::vector<double> matrixA(size * size);
        std::vector<double> matrixB(size * size);
        std::vector<double> result(size * size);

        fill_random(matrixA);
        fill_random(matrixB);

        auto [avg, stdev] = benchmark_function(
            [&]() {
                multiply_mm_naive(matrixA.data(), size, size, matrixB.data(),
                                  size, size, result.data());
            },
            runs);

        std::cout << "Size: " << size << "x" << size << " | Avg: " << avg
                  << " ms"
                  << " | StdDev: " << stdev << " ms\n";
    }

    std::cout << std::endl;

    std::cout << "Benchmarking multiply_mm_transposed_b:\n";

    for (int size : sizes) {
        std::vector<double> matrixA(size * size);
        std::vector<double> matrixB(size * size);
        std::vector<double> matrixB_transposed(size * size);
        std::vector<double> result(size * size);

        fill_random(matrixA);
        fill_random(matrixB);

        // Transpose matrixB into row-major matrixB_transposed
        for (int i = 0; i < size; ++i) {
            for (int j = 0; j < size; ++j) {
                matrixB_transposed[j * size + i] = matrixB[i * size + j];
            }
        }

        auto [avg, stdev] = benchmark_function(
            [&]() {
                multiply_mm_transposed_b(matrixA.data(), size, size,
                                         matrixB_transposed.data(), size, size,
                                         result.data());
            },
            runs);

        std::cout << "Size: " << size << "x" << size << " | Avg: " << avg
                  << " ms"
                  << " | StdDev: " << stdev << " ms\n";
    }

    std::cout << std::endl;

    return 0;
}
