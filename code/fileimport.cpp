#include "fileimport.h"

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

        curFile.close();
        std::cout << "=== Файл закрыт! ===\n" << std::endl;

    } catch (...) {
        throw std::runtime_error("Ошибка при открытии файла!");
    }
}

std::vector<std::vector<float>> fileImport(const std::string& fileName) {
    /* Записывает данные файла (в формате .csv, т.е. построчно через запятую)
     * в двумерным массив (std::vector). Обрабатывает как файлы с метками классов (в виде целого числа),
     * так и без метки. */

    std::fstream curFile;
    std::string line;
    std::vector<std::vector<float>> data;

    curFile.open(fileName);

    if (!curFile.is_open()) {
        throw std::runtime_error("Не удаётся открыть файл: " + fileName);
    }

    try {
        while (std::getline(curFile, line)) {

            if (line.empty() || line[0] == '#') {
                continue;
            }

            std::vector<float> row;
            std::stringstream ss(line);
            std::string num;

            while (std::getline(ss, num, ',')) {
                row.push_back(std::stof(num));
            }

            data.push_back(row);

        }
        return data;
    } catch (...) {
        throw std::runtime_error("Ошибка при открытии файла!");
    }
}
