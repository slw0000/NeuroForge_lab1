#include "layer.h"



Layer::Layer(int inputSize, int outputSize,
    ActivationFunc activationFunc, ActivationFunc activationFuncDerivative):

    weights(outputSize, inputSize),
    inputSize(inputSize), outputSize(outputSize),
    activationFunc(activationFunc),
    activationFuncDerivative(activationFuncDerivative){

    for (size_t i = 0; i < outputSize; i++) {
        for (size_t j = 0; j < inputSize; j++) {
            weights(i, j) = nnlab::genRandomNumber();
        }
    }
}


