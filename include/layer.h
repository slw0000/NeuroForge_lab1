#pragma once

#include "matrix_realization.h"
#include "utils.h"
#include <functional>

using ActivationFunc = std::function<double(double)>;

struct Layer {

    nnlab::Matrix weights;
    int inputSize;
    int outputSize;
    ActivationFunc activationFunc;
    ActivationFunc activationFuncDerivative;

    Layer(int inputSize, int outputSize,
        ActivationFunc activationFunc = nnlab::sigmoid,
        ActivationFunc activationFuncDerivative = nnlab::sigmoidDerivative);


};