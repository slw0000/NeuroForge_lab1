#include <iostream>

#include "matrix_realization.h"


int main() {
    try {
        // Создаем матрицу 2x3
        Matrix A(2, 3);
        A(0, 0) = 1; A(0, 1) = 2; A(0, 2) = 3;
        A(1, 0) = 4; A(1, 1) = 5; A(1, 2) = 6;

        // Создаем матрицу 3x2
        Matrix B(3, 2);
        B(0, 0) = 7; B(0, 1) = 8;
        B(1, 0) = 9; B(1, 1) = 10;
        B(2, 0) = 11; B(2, 1) = 12;

        std::cout << "Matrix A (2x3):\n" << A << std::endl;
        std::cout << "Matrix B (3x2):\n" << B << std::endl;

        // Умножение матриц (Результат 2x2)
        Matrix C = A * B;
        std::cout << "A * B (2x2):\n" << C << std::endl;

        // Сложение (нужны одинаковые размеры)
        Matrix D(2, 3, 1.0); // Матрица 2x3 из единиц
        Matrix E = A + D;
        std::cout << "A + 1 (2x3):\n" << E << std::endl;

        // Умножение на число
        Matrix F = A * 2.0;
        std::cout << "A * 2.0 (2x3):\n" << F << std::endl;

        // Пример ошибки (раскомментируйте, чтобы проверить)
        // Matrix Error = A + B; // Выбросит исключение

    } catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
