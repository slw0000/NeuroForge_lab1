#include "neural_network.h"

NeuralNetwork::NeuralNetwork(double learning_rate):
    weightsHidden1(2, 2), weightsHidden2(2, 2),
    weightsOutput(2, 1), learningRate(learning_rate) {

    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            weightsHidden1(i, j) = nnlab::genRandomNumber(-1.0, 1.0);
            weightsHidden2(i, j) = nnlab::genRandomNumber(-1.0, 1.0);
        }
    }

    for (size_t i = 0; i < 2; i++) {
        weightsOutput(i, 0) = nnlab::genRandomNumber(-1.0, 1.0);
    }
}

void NeuralNetwork::setWeights(std::vector<nnlab::Matrix> weights) {
    weightsHidden1 = weights[0];
    weightsHidden2 = weights[1];
    weightsOutput = weights[2];
}

std::vector<nnlab::Matrix> NeuralNetwork::getWeights() {
    std::vector weights = {weightsHidden1, weightsHidden2, weightsOutput};
    return weights;
}


