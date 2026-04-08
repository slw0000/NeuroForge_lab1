#include "../include/file_import.h"

#include <cmath>
#include <iostream>
#include <fstream>
#include <sstream>


namespace helperFuncs{
    std::fstream safeFileOpening(const std::string& fileName) {
        std::fstream curFile;
        curFile.open(fileName);

        if (!curFile.is_open()) {
            throw std::runtime_error("Не удаётся открыть файл: " + fileName);
        }
        return curFile;
    };

    std::vector<std::vector<double>> simpleParser(std::fstream& curFile) {

        std::string line;
        std::vector<std::vector<double>> data;

        while (std::getline(curFile, line)) {
            if (line.empty()) { continue; }

            std::vector<double> row;
            std::stringstream ss(line);
            std::string num;

            try {
                while (std::getline(ss, num, ',')) {
                    row.push_back(std::stod(num));
                }
            } catch (...) { throw std::invalid_argument("Неверный формат данных в строке: " + line); }

            data.push_back(row);
        }
        return data;
    }

}


void nnlab::fileToConsole(const std::string& fileName) {
    /* Построчно выводит в консоль содержимое текстового файла */

    std::string line;
    auto curFile = helperFuncs::safeFileOpening(fileName);

    std::cout << "=== Файл открыт: " << fileName <<" ===" << std::endl;
    while (std::getline(curFile, line)) {
        std::cout << line << std::endl;
    }
    std::cout << "=== Файл закрыт! ===\n" << std::endl;

}

std::vector<std::vector<double>> nnlab::fileImportRaw(const std::string& fileName) {
    /*
    Записывает данные файла (в формате .csv, т.е. построчно через запятую) в двумерным массив (std::vector).
    Обрабатывает как файлы с метками классов (в виде целого числа), так и без метки.
    Игнорирует заголовки столбцов таблицы.
    */

    std::string line;
    std::vector<std::vector<double>> data;

    auto curFile = helperFuncs::safeFileOpening(fileName);
    std::getline(curFile, line);

    data = helperFuncs::simpleParser(curFile);

    return data;

}

std::vector<nnlab::Matrix> nnlab::fileImportMatrixRaw(const std::string& fileName) {
    /*
    Записывает данные файла (в формате .csv, т.е. построчно через запятую)
    в массив Матриц (Matrix). Обрабатывает файлы без меток классов.
    Игнорирует заголовки столбцов таблицы.
    */

    std::string line;
    std::vector<Matrix> data;
    Matrix cords(2, 1);

    auto curFile = helperFuncs::safeFileOpening(fileName);
    std::getline(curFile, line);

    for (const auto& row: helperFuncs::simpleParser(curFile)) {
        if (row.size() == 2) {
            cords(0, 0) = row[0];
            cords(1, 0) = row[1];
            data.push_back(cords);
        } else {
            throw std::invalid_argument("Количество чисел в строке не равно 2!");
        }
    }

    return data;
}

std::pair<std::vector<nnlab::Matrix>, std::vector<int>> nnlab::fileImportMatrixLabel(const std::string& fileName) {
    /*
    Записывает данные файла (в формате .csv, т.е. построчно через запятую)
    в пару <массив Матриц (Matrix), массив меток классов (int)>. Обрабатывает файлы c метками классов.
    Игнорирует заголовки столбцов таблицы.
    */

    std::string line;
    std::vector<Matrix> dataCords;
    std::vector<int> dataLabels;
    Matrix cords(2, 1);
    const double eps = 1e-6;

    auto curFile = helperFuncs::safeFileOpening(fileName);
    std::getline(curFile, line);

    for (const auto& row: helperFuncs::simpleParser(curFile)) {
        if (row.size() == 3) {
            cords(0, 0) = row[0];
            cords(1, 0) = row[1];
            dataCords.push_back(cords);

            if (std::abs(std::round(row[2]) - row[2]) < eps) {
                dataLabels.emplace_back(int(row[2]));
            } else { throw std::invalid_argument("Метка класса должна быть целым числом: " + std::to_string(row[2])); }
        } else { throw std::invalid_argument("Количество чисел в строке не равно 3!"); }
    }

    return std::make_pair(dataCords, dataLabels);
}

void nnlab::fileSaveToCSV(const std::string& fileName, const std::vector<nnlab::Matrix>& cords, const std::vector<int>& label) {
    /* Сохраняет координаты (как с метками классов так и без) в текстовый файл в формате csv. */

    if (cords.empty()) { throw std::invalid_argument("Массив координат не может быть пустым!"); }
    if (!label.empty() && (cords.size() != label.size())) {
        throw std::invalid_argument("Размеры массива координат и массива меток классов должны совпадать!");
    }

    std::ofstream curFile(fileName,  std::ios::out | std::ios::trunc);

    if (!curFile.is_open()) {throw std::runtime_error("Не удаётся открыть файл: " + fileName);}

    if (label.empty()) { curFile << "x,y" << std::endl;
    } else { curFile << "x,y,label" << std::endl; }

    for (size_t i = 0; i < cords.size(); i++) {
        if (cords[i].rows() != 2 || cords[i].cols() != 1) {
            throw std::invalid_argument("Матрица должна быть размером 2x1!");
        }

        curFile << cords[i](0, 0) << "," << cords[i](1, 0);
        if (!label.empty()) { curFile << "," << label[i]; }

        curFile << std::endl;
    }
};


