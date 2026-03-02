#include "fileimport.h"

#include <iostream>
#include <fstream>
#include <sstream>

void fileToConsole(std::string const& fileName) {
    /* Построчно выводит в консоль содержимое текстового файла */

    std::fstream curFile;
    std::string line;

    curFile.open(fileName);

    if (curFile.is_open()) {
        std::cout << "=== Файл открыт: " << fileName <<" ===" << std::endl;
        while (std::getline(curFile, line)) {
            std::cout << line << std::endl;
        }

        curFile.close();
        std::cout << "=== Файл закрыт! ===" << std::endl;
    } else {
        std::cerr << "Ошибка при открытии файла!" << std::endl;
    }
}

std::vector<std::vector<float>> fileImport(std::string const& fileName) {
    /* Записывает данные файла (в формате .csv, т.е. построчно через запятую) в вектор векторов (std::vector) */

    std::fstream curFile;
    std::string line;
    std::vector<std::vector<float>> data;

    curFile.open(fileName);

    if (curFile.is_open()) {
        while (std::getline(curFile, line)) {
            std::vector<float> row;
            std::stringstream ss(line);
            std::string cord;

            while (std::getline(ss, cord, ',')) {
                row.push_back(std::stof(cord));
            }

            data.push_back(row);
        }
        return data;
    } else {
        std::cerr << "Ошибка при открытии файла!" << std::endl;
        return data;
    }
}
