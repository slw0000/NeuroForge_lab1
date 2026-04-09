#include "neural_network.h"

NeuralNetwork::NeuralNetwork(double learning_rate):
    weightsHidden1(2, 2), weightsHidden2(2, 2),
    weightsOutput(1, 2), learningRate(learning_rate) {

    for (size_t i = 0; i < 2; i++) {
        for (size_t j = 0; j < 2; j++) {
            weightsHidden1(i, j) = nnlab::genRandomNumber(-1.0, 1.0);
            weightsHidden2(i, j) = nnlab::genRandomNumber(-1.0, 1.0);
        }
    }

    for (size_t i = 0; i < 2; i++) {
        weightsOutput(0, i) = nnlab::genRandomNumber(-1.0, 1.0);
    }
}

void NeuralNetwork::setWeights(std::vector<nnlab::Matrix> weights) {
    weightsHidden1 = weights[0];
    weightsHidden2 = weights[1];
    weightsOutput = weights[2];
}

std::vector<nnlab::Matrix> NeuralNetwork::getWeights(){
    std::vector weights = {weightsHidden1, weightsHidden2, weightsOutput};
    return weights;
}

void NeuralNetwork::setLearningRate(double learning_rate) {
    if (learning_rate <= 0.0) {
        throw std::invalid_argument("learningRate должен быть больше 0!");
    }
    learningRate = learning_rate;
}

double NeuralNetwork::getLearningRate(){
    return learningRate;
}

double NeuralNetwork::forward(const nnlab::Matrix& vector) {

    nnlab::Matrix layer1 = weightsHidden1 * vector;
    layer1(0, 0) = nnlab::sigmoid(layer1(0, 0));
    layer1(1, 0) = nnlab::sigmoid(layer1(1, 0));

    nnlab::Matrix layer2 = weightsHidden2 * layer1;
    layer2(0, 0) = nnlab::sigmoid(layer2(0, 0));
    layer2(1, 0) = nnlab::sigmoid(layer2(1, 0));

    double output = nnlab::sigmoid((weightsOutput * layer2)(0, 0));

    return output;
}

std::vector<double> NeuralNetwork::predictProba(std::vector<nnlab::Matrix>& inputData) {
    std::vector<double> prediction(inputData.size());

    for (size_t i = 0; i < inputData.size(); i++) {
        prediction[i] = forward(inputData[i]);
    }

    return prediction;

}

std::vector<int> NeuralNetwork::predict(std::vector<nnlab::Matrix>& inputData) {
    std::vector<int> prediction(inputData.size());
    auto data = predictProba(inputData);

    for (size_t i = 0; i < data.size(); i++) {
        prediction[i] = data[i] > 0.5 ? 1 : 0;
    }

    return prediction;
}

void NeuralNetwork::train(std::pair<std::vector<nnlab::Matrix>, std::vector<int>>& trainData,
    const LossFunction& lossFunc, const LossFunction& lossDerivative, int maxEpochs, double minDelta, int patience) {

    // используем sigmoid на выходном слое и tanh на остальных, по умолчанию BinCrossEnt как Loss по умлочанию

    auto trainValues = trainData.first;
    auto trainPoints = trainData.second;

    double prevLoss = 0.0;
    double epochLoss = 0.0;
    int curPatience = 0;

    for (int epo = 0; epo <= maxEpochs; epo++) {
        epochLoss = 0.0;

        for (size_t i = 0; i < trainPoints.size(); i++) {

            // считаем output(x)
            auto vector = trainValues[i];
            nnlab::Matrix layer1 = weightsHidden1 * vector;
            layer1(0, 0) = nnlab::sigmoid(layer1(0, 0));
            layer1(1, 0) = nnlab::sigmoid(layer1(1, 0));
            nnlab::Matrix layer2 = weightsHidden2 * layer1;
            layer2(0, 0) = nnlab::sigmoid(layer2(0, 0));
            layer2(1, 0) = nnlab::sigmoid(layer2(1, 0));

            double output = nnlab::sigmoid((weightsOutput * layer2)(0, 0));

            // считаем delta(x)
            auto deltaOut = - lossDerivative(output, trainPoints[i]) * nnlab::sigmoidDerivative(output);

            auto delta2 = nnlab::Matrix(2, 1);
            delta2(0, 0) = nnlab::sigmoidDerivative(layer2(0, 0)) * deltaOut * weightsOutput(0, 0);
            delta2(1, 0) = nnlab::sigmoidDerivative(layer2(1, 0)) * deltaOut * weightsOutput(0, 1);

            auto delta1 = nnlab::Matrix(2, 1);
            delta1(0, 0) = nnlab::sigmoidDerivative(layer1(0, 0)) * (delta2(0, 0) * weightsHidden2(0, 0) +
                                                                            delta2(1, 0) * weightsHidden2(1, 0));
            delta1(1, 0) = nnlab::sigmoidDerivative(layer1(1, 0)) * (delta2(0, 0) * weightsHidden2(0, 1) +
                                                                            delta2(1, 0) * weightsHidden2(1, 1));

            // считаем p(w)
            auto p_hidden1 = nnlab::Matrix(2, 2);
            auto p_hidden2 = nnlab::Matrix(2, 2);
            auto p_output = nnlab::Matrix(1, 2);

            p_hidden1(0, 0) = delta1(0, 0) * vector(0, 0);
            p_hidden1(0, 1) = delta1(0, 0) * vector(1, 0);
            p_hidden1(1, 0) = delta1(1, 0) * vector(0, 0);
            p_hidden1(1, 1) = delta1(1, 0) * vector(1, 0);

            p_hidden2(0, 0) = delta2(0, 0) * layer1(0, 0);
            p_hidden2(0, 1) = delta2(0, 0) * layer1(1, 0);
            p_hidden2(1, 0) = delta2(1, 0) * layer1(0, 0);
            p_hidden2(1, 1) = delta2(1, 0) * layer1(1, 0);

            p_output(0, 0) = deltaOut * layer2(0, 0);
            p_output(0, 1) = deltaOut * layer2(1, 0);

            // считаем обновленные веса
            weightsHidden1 = weightsHidden1 + p_hidden1 * learningRate;
            weightsHidden2 = weightsHidden2 + p_hidden2 * learningRate;
            weightsOutput = weightsOutput + p_output * learningRate;

            epochLoss += lossFunc(output, trainPoints[i]);
        }

        if (std::abs((prevLoss - epochLoss) / trainPoints.size()) < minDelta) {
            curPatience += 1;
        } else { curPatience = 0; }

        if (curPatience >= patience) {
            double Loss = epochLoss / trainPoints.size() ;
            std::cout << "Epoch " << epo << ": Loss = " << Loss << std::endl;
            break;
        }

        if (epo % 100 == 0 ) {
            double Loss = epochLoss / trainPoints.size() ;
            std::cout << "Epoch " << epo << ": Loss = " << Loss << std::endl;
        }

        prevLoss = epochLoss;
    }
}


