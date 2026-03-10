#include "fileimport.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>

void fileToConsole(const std::string& fileName) {
    /* Построчно выводит в консоль содержимое текстового файла */

    std::fstream curFile;
    std::string line;

    curFile.open(fileName);

    if (!curFile.is_open()) {
        throw std::runtime_error("Не удаётся открыть файл: " + fileName);
    }

    try {

        std::cout << "=== Файл открыт: " << fileName <<" ===" << std::endl;
        while (std::getline(curFile, line)) {
            std::cout << line << std::endl;
        }

        std::cout << "=== Файл закрыт! ===\n" << std::endl;

    } catch (...) {
        throw std::runtime_error("Ошибка при открытии файла!");
    }
}

std::vector<std::vector<double>> fileImportRaw(const std::string& fileName) {
    /*
    Записывает данные файла (в формате .csv, т.е. построчно через запятую) в двумерным массив (std::vector).
    Обрабатывает как файлы с метками классов (в виде целого числа), так и без метки.
    */

    std::fstream curFile;
    std::string line;
    std::vector<std::vector<double>> data;

    curFile.open(fileName);

    if (!curFile.is_open()) {
        throw std::runtime_error("Не удаётся открыть файл: " + fileName);
    }

    try {
        while (std::getline(curFile, line)) {

            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::vector<double> row;
            std::stringstream ss(line);
            std::string num;

            try {
                while (std::getline(ss, num, ',')) {
                    row.push_back(std::stod(num));
                }
            } catch (...) {
                throw std::runtime_error("неверный формат данных!");
            }

            data.push_back(row);

        }
        return data;
    } catch (const std::exception& error_message) {
        throw std::runtime_error("Ошибка при открытии файла: " + std::string(error_message.what()));
    }
}

std::vector<Matrix> fileImportMatrixRaw(const std::string& fileName) {
    /*
    Записывает данные файла (в формате .csv, т.е. построчно через запятую)
    в массив Матриц (Matrix). Обрабатывает файлы без меток классов.
    */

    std::fstream curFile;
    std::string line;
    std::vector<Matrix> data;

    curFile.open(fileName);

    if (!curFile.is_open()) {
        throw std::runtime_error("Не удаётся открыть файл: " + fileName);
    }

    try {
        while (std::getline(curFile, line)) {

            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::vector<double> row;
            Matrix cords(2, 1);
            std::stringstream ss(line);
            std::string num;

            try {
                while (std::getline(ss, num, ',')) {
                    row.push_back(std::stod(num));
                }
            } catch (...) {
                throw std::runtime_error("неверный формат данных!");
            }

            if (row.size() == 2) {
                cords(0, 0) = row[0];
                cords(1, 0) = row[1];
                data.push_back(cords);
            } else {
                throw std::runtime_error("количество чисел в строке не равно 2!");
            }

        }
        return data;
    } catch (const std::exception& error_message) {
        throw std::runtime_error("Ошибка при открытии файла: " + std::string(error_message.what()));
    }
}

std::vector<std::pair<Matrix, int>> fileImportMatrixLabel(const std::string& fileName) {
    /*
    Записывает данные файла (в формате .csv, т.е. построчно через запятую)
    в массив пар <Матрица (Matrix), метка класса (int)>. Обрабатывает файлы c метками классов.
    */

    std::fstream curFile;
    std::string line;
    std::vector<std::pair<Matrix, int>> data;
    const double eps = 1e-6;

    curFile.open(fileName);

    if (!curFile.is_open()) {
        throw std::runtime_error("Не удаётся открыть файл: " + fileName);
    }

    try {
        while (std::getline(curFile, line)) {

            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::vector<double> row;
            Matrix cords(2, 1);
            std::stringstream ss(line);
            std::string num;

            try {
                while (std::getline(ss, num, ',')) {
                    row.push_back(std::stod(num));
                }
            } catch (...) {
                throw std::runtime_error("неверный формат данных!");
            }

            if (row.size() == 3) {
                cords(0, 0) = row[0];
                cords(1, 0) = row[1];
                if (std::abs(std::round(row[2]) - row[2]) < eps) {
                    data.emplace_back(cords, int(row[2]));
                } else {
                    throw std::runtime_error("метка класса должна быть целым числом!");
                }
            } else {
                throw std::runtime_error("количество чисел в строке не равно 3!");
            }

        }
        return data;
    } catch (const std::exception& error_message) {
        throw std::runtime_error("Ошибка при открытии файла: " + std::string(error_message.what()));
    }
}
