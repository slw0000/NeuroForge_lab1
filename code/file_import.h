#pragma once

#include <string>
#include <vector>
#include "matrix_realization.h"

namespace nnlab {
    void fileToConsole(const std::string& fileName);

    std::vector<std::vector<double>> fileImportRaw(const std::string& fileName);

    std::vector<Matrix> fileImportMatrixRaw(const std::string& fileName);

    std::pair<std::vector<Matrix>, std::vector<int>> fileImportMatrixLabel(const std::string& fileName);

    void fileSaveToCSV(const std::string& fileName, const std::vector<Matrix>& cords, const std::vector<int>& label = {});
}


