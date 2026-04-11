#pragma once

#include "matrix_realization.h"
#include "utils.h"
#include "layer.h"

using LossFunction = std::function<double(double, int)>;

class NeuralNetwork {

    std::vector<Layer> weights;
    std::vector<int> neuralNetStruct;

public:

    NeuralNetwork(const std::vector<int>& neuralNetStruct = {2, 2, 2, 1},
        const std::vector<ActivationFunc>& activationFuncList = {},
        const std::vector<ActivationFunc>& activationFuncDerivativeList = {});

    void train(std::pair<std::vector<nnlab::Matrix>, std::vector<int>>& trainData,
        const LossFunction& lossFunc = nnlab::mseLoss,
        const LossFunction& lossDerivative = nnlab::mseDerivative,
        int maxEpochs = 10000, double learningRate = 0.1, double minDelta = 0.00001,
        int patience = 10);

    std::vector<nnlab::Matrix> forward(nnlab::Matrix vector);

    std::vector<double> predictProba(std::vector<nnlab::Matrix>& inputData);
    std::vector<int> predict(std::vector<nnlab::Matrix>& inputData);

    void setWeights(std::vector<nnlab::Matrix>& weights);
    std::vector<nnlab::Matrix> getWeights();
};


