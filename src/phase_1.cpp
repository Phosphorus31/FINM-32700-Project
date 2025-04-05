#include <iostream>

void multiply_mv_row_major(const double* matrix, int rows, int cols,
                           const double* vector, double* result) {
    if (!matrix || !vector || !result) {
        throw std::invalid_argument(
            "Null pointer passed to multiply_mv_row_major");
    }
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument(
            "Invalid matrix dimensions in multiply_mv_row_major");
    }

    for (int i = 0; i < rows; i++) {
        double sum = 0.0;

        for (int j = 0; j < cols; j++) {
            sum += matrix[i * cols + j] * vector[j];
        }

        result[i] = sum;
    }
}

void multiply_mv_col_major(const double* matrix, int rows, int cols,
                           const double* vector, double* result) {
    if (!matrix || !vector || !result) {
        throw std::invalid_argument(
            "Null pointer passed to multiply_mv_col_major");
    }
    if (rows <= 0 || cols <= 0) {
        throw std::invalid_argument(
            "Invalid matrix dimensions in multiply_mv_col_major");
    }

    for (int i = 0; i < rows; ++i) {
        result[i] = 0.0;
    }

    for (int j = 0; j < cols; ++j) {
        for (int i = 0; i < rows; ++i) {
            result[i] += matrix[j * rows + i] * vector[j];
        }
    }
}

void multiply_mm_naive(const double* matrixA, int rowsA, int colsA,
                       const double* matrixB, int rowsB, int colsB,
                       double* result) {
    if (!matrixA || !matrixB || !result) {
        throw std::invalid_argument("Null pointer passed to multiply_mm_naive");
    }
    if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0) {
        throw std::invalid_argument(
            "Invalid matrix dimensions in multiply_mm_naive");
    }
    if (colsA != rowsB) {
        throw std::invalid_argument(
            "Incompatible matrix dimensions for multiplication in "
            "multiply_mm_naive");
    }

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            double sum = 0.0;
            for (int k = 0; k < colsA; ++k) {
                sum += matrixA[i * colsA + k] * matrixB[k * colsB + j];
            }
            result[i * colsB + j] = sum;
        }
    }
}

void multiply_mm_transposed_b(const double* matrixA, int rowsA, int colsA,
                              const double* matrixB_transposed, int rowsB,
                              int colsB, double* result) {
    if (!matrixA || !matrixB_transposed || !result) {
        throw std::invalid_argument(
            "Null pointer passed to multiply_mm_transposed_b");
    }
    if (rowsA <= 0 || colsA <= 0 || rowsB <= 0 || colsB <= 0) {
        throw std::invalid_argument(
            "Invalid matrix dimensions in multiply_mm_transposed_b");
    }
    if (colsA != rowsB) {
        throw std::invalid_argument(
            "Incompatible matrix dimensions for multiplication in "
            "multiply_mm_transposed_b");
    }

    for (int i = 0; i < rowsA; ++i) {
        for (int j = 0; j < colsB; ++j) {
            double sum = 0.0;
            for (int k = 0; k < colsA; ++k) {
                sum +=
                    matrixA[i * colsA + k] * matrixB_transposed[j * rowsB + k];
            }
            result[i * colsB + j] = sum;
        }
    }
}
