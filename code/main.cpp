#include <iostream>

#include "matrix_realization.h"
#include "fileimport.h"

void runAllMatrixTests();

int main(int argc, char* argv[]) {
    bool runTests = false;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--test") {
            runTests = true;
            break;
        }
    }

    if (runTests) {
        std::cout << "Matrix tests: \n" << std::endl;
        try {
            runAllMatrixTests();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        } catch (...) {
            std::cerr << "Unknown error" << std::endl;
            return 1;
        }
    }
    else{
        std::cout << "Начальная точка \n" << std::endl;

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
            G(0, 0) = 1; G(0, 1) = 2; G(0, 2) = 3;
            G(1, 0) = 4; G(1, 1) = 5; G(1, 2) = 6;

            std::cout << "Matrix G (2x3):\n" << G << std::endl;
            Matrix T = G.transpose();
            std::cout << "Transposed matrix G (3x2):\n" << T << std::endl;

        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        }

        // =======================================================================================================================================

        /*
        ====== Пример использования утилиты открытия файла ======
        */


        fileToConsole("../data/example.txt");
        fileToConsole("../data/example.csv");

        std::cout << "Считывание в двумерный массив:" << std::endl;
        auto data = fileImportRaw("../data/example.csv");
        for (auto& row : data) {
            std::cout << row[0] << " " << row[1] << std::endl;
        }

        std::cout << "Считывание в массив матриц:" << std::endl;
        auto data2 = fileImportMatrixRaw("../data/example.csv");
        for (auto& row : data2) {
            std::cout << row << std::endl;
        }

        std::cout << "Считывание в массив <матрица, метка класса>:" << std::endl;
        auto data3 = fileImportMatrixLabel("../data/exampleLabel.csv");
        for (auto& row : data3) {
            std::cout << row.first << row.second << std::endl;
        }

        try {
            auto dataError = fileImportMatrixRaw("../data/WrongName.csv");
        } catch (const std::exception& error_message) {
            std::cerr << "Пойманная ошибка: " << error_message.what() << std::endl;
        }

        try {
            auto dataError = fileImportMatrixRaw("../data/fileErrorExample.csv");
        } catch (const std::exception& error_message) {
            std::cerr << "Пойманная ошибка: " << error_message.what() << std::endl;
        }

        try {
            auto dataError = fileImportRaw("../data/fileErrorExample.csv");
        } catch (const std::exception& error_message) {
            std::cerr << "Пойманная ошибка: " << error_message.what() << std::endl;
        }

        try {
            auto dataError = fileImportMatrixLabel("../data/fileErrorExample.csv");
        } catch (const std::exception& error_message) {
            std::cerr << "Пойманная ошибка: " << error_message.what() << std::endl;
        }


        return 0;
    }
}