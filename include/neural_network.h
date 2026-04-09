#pragma once

#include "matrix_realization.h"
#include "utils.h"


// TODO:
// activations funcs and derivatives,
// predict methods,
// train methods (!),

class NeuralNetwork {
private:

    nnlab::Matrix weightsHidden1;
    nnlab::Matrix weightsHidden2;
    nnlab::Matrix weightsOutput;

    double learningRate;

public:

    NeuralNetwork(double learning_rate = 0.1);

    void train(std::pair<std::vector<nnlab::Matrix>, std::vector<int>> trainData, int epochs = 100);

    std::vector<int> predict(std::vector<nnlab::Matrix> inputData);
    std::vector<double> predict_proba(std::vector<nnlab::Matrix> inputData);

    void setWeights(std::vector<nnlab::Matrix> weights);
    std::vector<nnlab::Matrix> getWeights();

    void setLearningRate(double learningRate);
    double getLearningRate();
};


