#include "layer.h"
#include <cmath>

Layer::Layer(int inputSize, int outputSize,
    ActivationFunc activationFunc, ActivationFunc activationFuncDerivative):

    weights(outputSize, inputSize),
    inputSize(inputSize), outputSize(outputSize),
    activationFunc(activationFunc),
    activationFuncDerivative(activationFuncDerivative) {

    double limit;
    if (outputSize == 1) {
        limit = 0.01;
    } else {
        limit = sqrt(6.0 / (inputSize + outputSize));
    }

    for (size_t i = 0; i < outputSize; i++) {
        for (size_t j = 0; j < inputSize; j++) {
            weights(i, j) = nnlab::genRandomNumber(-limit, limit);
        }
    }
}



