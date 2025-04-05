#include <iostream>

void multiply_mv_row_major(const double* matrix, int rows, int cols,
                           const double* vector, double* result) {
    // TODO: error handling

    for (int i = 0; i < rows; i++) {
        double sum = 0.0;

        for (int j = 0; j < cols; j++) {
            sum += matrix[i * cols + j] * vector[j];
        }

        result[i] = sum;
    }
}
