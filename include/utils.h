#pragma once

#include "matrix_realization.h"

namespace nnlab {

    double genRandomNumber(double min, double max);
    int genRandomInt(int min, int max);

    std::pair<std::vector<Matrix>, std::vector<int>> genBinClassifyDataset(
        int count, double noise = 0.1,
        double x0_center = 0.3, double y0_center = 0.3,
        double x1_center = 0.7, double y1_center = 0.7);

    double sigmoid(double x);
    double sigmoidDerivative(double x);

    double relu(double x);
    double reluDerivative(double x);
}

