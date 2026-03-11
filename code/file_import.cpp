#include "file_import.h"

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
    Игнорирует заголовки столбцов таблицы.
    */

    std::fstream curFile;
    std::string line;
    std::vector<std::vector<double>> data;

    curFile.open(fileName);

    if (!curFile.is_open()) {
        throw std::runtime_error("Не удаётся открыть файл: " + fileName);
    }

    try {
        if (std::getline(curFile, line)) {}

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
                throw std::invalid_argument("неверный формат данных!");
            }

            data.push_back(row);

        }
        return data;
    } catch (const std::runtime_error& error_message) {
        throw std::runtime_error("Ошибка при открытии файла: " + std::string(error_message.what()));
    } catch (const std::invalid_argument& error_message) {
        throw std::invalid_argument("Ошибка при открытии файла: " + std::string(error_message.what()));
    }
}

std::vector<Matrix> fileImportMatrixRaw(const std::string& fileName) {
    /*
    Записывает данные файла (в формате .csv, т.е. построчно через запятую)
    в массив Матриц (Matrix). Обрабатывает файлы без меток классов.
    Игнорирует заголовки столбцов таблицы.
    */

    std::fstream curFile;
    std::string line;
    std::vector<Matrix> data;

    curFile.open(fileName);

    if (!curFile.is_open()) {
        throw std::runtime_error("Не удаётся открыть файл: " + fileName);
    }

    try {
        if (std::getline(curFile, line)) {}

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
                throw std::invalid_argument("неверный формат данных!");
            }

            if (row.size() == 2) {
                cords(0, 0) = row[0];
                cords(1, 0) = row[1];
                data.push_back(cords);
            } else {
                throw std::invalid_argument("количество чисел в строке не равно 2!");
            }

        }
        return data;
    } catch (const std::runtime_error& error_message) {
        throw std::runtime_error("Ошибка при открытии файла: " + std::string(error_message.what()));
    } catch (const std::invalid_argument& error_message) {
        throw std::invalid_argument("Ошибка при открытии файла: " + std::string(error_message.what()));
    }
}

std::pair<std::vector<Matrix>, std::vector<int>> fileImportMatrixLabel(const std::string& fileName) {
    /*
    Записывает данные файла (в формате .csv, т.е. построчно через запятую)
    в пару <массив Матриц (Matrix), массив меток классов (int)>. Обрабатывает файлы c метками классов.
    Игнорирует заголовки столбцов таблицы.
    */

    std::fstream curFile;
    std::string line;
    std::vector<Matrix> dataCords;
    std::vector<int> dataLabels;
    const double eps = 1e-6;

    curFile.open(fileName);

    if (!curFile.is_open()) {
        throw std::runtime_error("Не удаётся открыть файл: " + fileName);
    }

    try {
        if (std::getline(curFile, line)) {}

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
                throw std::invalid_argument("неверный формат данных!");
            }

            if (row.size() == 3) {
                cords(0, 0) = row[0];
                cords(1, 0) = row[1];
                dataCords.push_back(cords);
                if (std::abs(std::round(row[2]) - row[2]) < eps) {
                    dataLabels.emplace_back(int(row[2]));
                } else {
                    throw std::invalid_argument("метка класса должна быть целым числом!");
                }
            } else {
                throw std::invalid_argument("количество чисел в строке не равно 3!");
            }

        }
        return std::make_pair(dataCords, dataLabels);
    } catch (const std::runtime_error& error_message) {
        throw std::runtime_error("Ошибка при открытии файла: " + std::string(error_message.what()));
    } catch (const std::invalid_argument& error_message) {
        throw std::invalid_argument("Ошибка при открытии файла: " + std::string(error_message.what()));
    }
}

void fileSaveToCSV(const std::string& fileName, const std::vector<Matrix>& cords, const std::vector<int>& label) {
    /* Сохраняет координаты (как с метками классов так и без) в текстовый файл в формате csv. */
    if (cords.empty()) {
        throw std::invalid_argument("Массив координат не может быть пустым!");}
    if (!label.empty() && (cords.size() != label.size())) {
        throw std::invalid_argument("Размеры массива координат и массива меток классов должны совпадать!");}

    std::ofstream curFile(fileName);

    if (!curFile.is_open()) {throw std::runtime_error("Не удаётся открыть файл: " + fileName);}

    if (label.empty()) {
        curFile << "x, y" << std::endl;
    } else {
        curFile << "x, y, label" << std::endl;
    }

    for (size_t i = 0; i < cords.size(); i++) {
        if (cords[i].rows() != 2 || cords[i].cols() != 1) {
            throw std::invalid_argument("Матрица должна быть размером 2x1!");
        }

        curFile << cords[i](0, 0) << ", " << cords[i](1, 0);
        if (!label.empty()) {
            curFile << ", " << static_cast<int>(label[i]);
        }

        curFile << std::endl;
    }
};


