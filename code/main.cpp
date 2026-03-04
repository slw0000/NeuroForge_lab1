#include <iostream>

#include "matrix_realization.h"


int main() {
    try {
        Matrix A(2, 3);
        A(0, 0) = 1; A(0, 1) = 2; A(0, 2) = 3;
        A(1, 0) = 4; A(1, 1) = 5; A(1, 2) = 6;

        Matrix B(3, 2);
        B(0, 0) = 7; B(0, 1) = 8;
        B(1, 0) = 9; B(1, 1) = 10;
        B(2, 0) = 11; B(2, 1) = 12;

        std::cout << "Matrix A (2x3):\n" << A << std::endl;
        std::cout << "Matrix B (3x2):\n" << B << std::endl;

        Matrix C = A * B;
        std::cout << "A * B (2x2):\n" << C << std::endl;

        Matrix D(2, 3, 1.0);
        Matrix E = A + D;
        std::cout << "A + 1 (2x3):\n" << E << std::endl;

        Matrix F = A * 2.0;
        std::cout << "A * 2.0 (2x3):\n" << F << std::endl;

        Matrix G(2, 3);
        A(0, 0) = 1; A(0, 1) = 2; A(0, 2) = 3;
        A(1, 0) = 4; A(1, 1) = 5; A(1, 2) = 6;

        std::cout << "Matrix G (2x3):\n" << G << std::endl;
        Matrix T = G.transpose();
        std::cout << "Transposed matrix G (3x2):\n" << T << std::endl;

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
