#include <iostream>

#include "fileimport.h"


int main() {
    std::cout << "Начальная точка \n" << std::endl;

    /*
    ====== Пример использования утилиты открытия файла ======
    */

    fileToConsole("../data/example.txt");
    fileToConsole("../data/example.csv");

    auto data = fileImport("../data/example.csv");
    for (auto& row : data) {
        std::cout << row[0] << " " << row[1] << std::endl;
    }

    /*
    Пример выброса исключения

    auto dataError = fileImport("../data/fileErroreExample.csv");
    for (auto& row : data) {
        std::cout << row[0] << " " << row[1] << std::endl;
    }
    */


    return 0;
}