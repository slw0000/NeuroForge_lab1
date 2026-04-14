#pragma once

#include "matrix_realization.h"

using Dataset = std::pair<std::vector<nnlab::Matrix>, std::vector<int>>;

namespace nnlab {

    double genRandomNumber(double min = -1.0, double max = 1.0);
    int genRandomInt(int min = -1, int max = 1);

    Dataset genBinClassifyDataset(
        int count, double noise = 0.1,
        double x0_center = 0.3, double y0_center = 0.3,
        double x1_center = 0.7, double y1_center = 0.7);

    Dataset minMaxNormalization(
        const std::pair<std::vector<Matrix>, std::vector<int>>& data);

    double sigmoid(double x);
    double sigmoidDerivative(double x);

    double relu(double x);
    double reluDerivative(double x);

    double tanh(double x);
    double tanhDerivative(double x);

    double mseLoss(double predictValue, int correctValue);
    double mseDerivative(double predictValue, int correctValue);

    double bceLoss(double predictValue, int correctValue);
    double bceDerivative(double predictValue, int correctValue);

    double accuracy(const std::vector<int>& predict, const std::vector<int>& correct);
    double rocAuc(const std::vector<double>& predictProba, const std::vector<int>& correct);
}

