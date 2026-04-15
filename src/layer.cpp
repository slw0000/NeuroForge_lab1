#include "layer.h"
#include <cmath>
#include <random>

Layer::Layer(int inputSize, int outputSize,
    ActivationFunc activationFunc, ActivationFunc activationFuncDerivative):

    weights(outputSize, inputSize),
    inputSize(inputSize), outputSize(outputSize),
    activationFunc(activationFunc),
    activationFuncDerivative(activationFuncDerivative) {

    double std_dev = std::sqrt(2.0 / inputSize);

    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::normal_distribution<> dist(0.0, std_dev);

    for (size_t i = 0; i < outputSize; i++) {
        for (size_t j = 0; j < inputSize; j++) {
            weights(i, j) = dist(gen);
        }
    }
}



