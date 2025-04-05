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
        return 0;
    } else {
        std::cout << "Test failed!" << std::endl;
        return 1;
    }
}
