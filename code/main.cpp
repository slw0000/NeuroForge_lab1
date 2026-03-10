#include <iostream>

#include "matrix_realization.h"
#include "file_import.h"

void runAllMatrixTests();
void runAllFileImportTests();

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

        std::cout << "\nFile import tests: \n" << std::endl;
        try {
            runAllFileImportTests();
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

        /*
        ====== Пример использования класса матриц ======
        */

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

        std::cout << "Считывание в пару <массив матриц, массив меток класса>:" << std::endl;
        auto data3 = fileImportMatrixLabel("../data/exampleLabel.csv");
        for (int i; i < data3.first.size(); i++) {
            std::cout << data3.first[i] << data3.second[i] << std::endl;
        }

        std::cout << "Запись координат и меток в файл:\nДанные на входе:" << std::endl;
        Matrix A(2, 1), B(2, 1), C(2, 1);
        A(0, 0) = 1; A(1, 0) = 3;
        B(0, 0) = 2; B(1, 0) = -4;
        C(0, 0) = 0; C(1, 0) = 9;
        std::vector<Matrix> data4cords = {A, B, C};
        std::vector<int> data4label = {1, 2, 3};
        std::cout << data4cords[0] << " " << data4cords[1] << " " << data4cords[2] << std::endl;
        std::cout << data4label[0] << " " << data4label[1] << " " << data4label[2] << std::endl;

        fileSaveToCSV("../data/saveExample.csv", data4cords, data4label);
        std::cout << "Файл успешно создан!" << std::endl;

        return 0;
    }
}