#include <cstdlib>  // posix_memalign
#include <iostream>
#include <memory>  // unique_ptr
#include <random>  // random_device, mt19937, uniform_real_distribution

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

void fill_random(double* data, size_t size) {
    std::mt19937 gen(42);  // Fixed seed for reproducibility
    std::uniform_real_distribution<> dis(0.0, 1.0);

    for (size_t i = 0; i < size; ++i) {
        data[i] = dis(gen);
    }
}

// Aligned allocator
double* aligned_alloc64(size_t size) {
    void* ptr = nullptr;
    if (posix_memalign(&ptr, 64, size * sizeof(double)) != 0) {
        return nullptr;
    }
    return reinterpret_cast<double*>(ptr);
}

int main() {
    std::vector<int> sizes = {256, 1024, 4096, 16384};
    const int runs = 10;

    std::cout << "Benchmarking multiply_mv_row_major:\n";

    for (int size : sizes) {
        auto matrix = std::unique_ptr<double[], decltype(&std::free)>(
            aligned_alloc64(size * size), &std::free);
        fill_random(matrix.get(), size * size);

        auto vector_in = std::unique_ptr<double[], decltype(&std::free)>(
            aligned_alloc64(size), &std::free);
        fill_random(matrix.get(), size);

        auto result = std::unique_ptr<double[], decltype(&std::free)>(
            aligned_alloc64(size), &std::free);

        auto [avg, stdev] = benchmark_function(
            [&]() {
                multiply_mv_row_major(matrix.get(), size, size, vector_in.get(),
                                      result.get());
            },
            runs);

        std::cout << "Size: " << size << "x" << size << " | Avg: " << avg
                  << " ms"
                  << " | StdDev: " << stdev << " ms\n";
    }

    std::cout << std::endl;

    return 0;
}