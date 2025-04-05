#include <iostream>

#include "../src/phase_1.cpp"

int main() {
    const int rows = 2;
    const int cols = 3;

    double matrix[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0};
    double vector[] = {1.0, 2.0, 3.0};

    double result[rows];

    multiply_mv_row_major(matrix, rows, cols, vector, result);

    if (result[0] == 14.0 && result[1] == 32.0) {
        std::cout << "Test passed!" << std::endl;
        // return 0;
    } else {
        std::cout << "Test failed!" << std::endl;
        // return 1;
    }

    // column-major storage
    // [1 2 3]
    // [4 5 6]
    double matrix_2[] = {1.0, 4.0, 2.0, 5.0, 3.0, 6.0};
    
    multiply_mv_col_major(matrix_2, rows, cols, vector, result);
    if (result[0] == 14.0 && result[1] == 32.0) {
        std::cout << "Test passed!" << std::endl;
        // return 0;
    } else {
        std::cout << "Test failed!" << std::endl;
        // return 1;
    }

    const int rowsA = 2, colsA = 3;
    const int rowsB = 3, colsB = 2;

    double A[] = {1, 2, 3, 4, 5, 6};
    double B[] = {7, 8, 9, 10, 11, 12};
    double C[4];

    multiply_mm_naive(A, rowsA, colsA, B, rowsB, colsB, C);

    bool pass = C[0] == 58.0 && C[1] == 64.0 && C[2] == 139.0 && C[3] == 154.0;

    if (pass) {
        std::cout << "Matrix-Matrix Naive test passed\n";
        // return 0;
    } else {
        std::cout << "Matrix-Matrix Naive test failed\n";
        // return 1;
    }

    const int rowsA_2 = 2, colsA_2 = 3;
    const int rowsB_2 = 3, colsB_2 = 2;

    double A_2[] = {1, 2, 3, 4, 5, 6};              // row-major
    double B_transposed[] = {7, 9, 11, 8, 10, 12};  // 2x3, row-major

    multiply_mm_transposed_b(A_2, rowsA_2, colsA_2, B_transposed, rowsB_2,
                             colsB_2, C);

    bool pass_2 =
        C[0] == 58.0 && C[1] == 64.0 && C[2] == 139.0 && C[3] == 154.0;

    if (pass_2) {
        std::cout << "Matrix-Matrix (Transposed B) test passed\n";
    return 0;
    } else {
        std::cout << "Matrix-Matrix (Transposed B) test failed\n";
        return 1;
    }
}

