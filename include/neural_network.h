#pragma once

#include "matrix_realization.h"
#include "utils.h"


// TODO:
// train method (!)

class NeuralNetwork {
private:

    nnlab::Matrix weightsHidden1;
    nnlab::Matrix weightsHidden2;
    nnlab::Matrix weightsOutput;

    double learningRate;

public:

    NeuralNetwork(double learning_rate = 0.01);

    using LossFunction = std::function<double(double, int)>;

    void train(std::pair<std::vector<nnlab::Matrix>, std::vector<int>>& trainData,
        const LossFunction& lossFunc = nnlab::bceLoss,
        const LossFunction& lossDerivative = nnlab::bceDerivative,
        int maxEpochs = 10000, double minDelta = 0.00001,
        int patience = 10);

    double forward(const nnlab::Matrix& vector);

    std::vector<double> predictProba(std::vector<nnlab::Matrix>& inputData);
    std::vector<int> predict(std::vector<nnlab::Matrix>& inputData);

    void setWeights(std::vector<nnlab::Matrix> weights);
    std::vector<nnlab::Matrix> getWeights();

    void setLearningRate(double learningRate);
    double getLearningRate();
};


