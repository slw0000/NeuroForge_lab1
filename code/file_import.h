#pragma once

#include <string>
#include <vector>
#include "matrix_realization.h"


void fileToConsole(const std::string& fileName);

std::vector<std::vector<double>> fileImportRaw(const std::string& fileName);

std::vector<Matrix> fileImportMatrixRaw(const std::string& fileName);

std::pair<std::vector<Matrix>, std::vector<int>> fileImportMatrixLabel(const std::string& fileName);



