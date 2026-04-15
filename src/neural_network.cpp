#include "neural_network.h"

NeuralNetwork::NeuralNetwork(const std::vector<int>& neuralNetStruct,
    const std::vector<ActivationFunc>& activationFuncList,
    const std::vector<ActivationFunc>& activationFuncDerivativeList):

    neuralNetStruct(neuralNetStruct) {

    if (neuralNetStruct.size() < 2) {
        throw std::invalid_argument("Структура перцептрона должна содержать минимум 2 слоя!");
    }

    if (activationFuncList.size() != activationFuncDerivativeList.size()) {
        throw std::invalid_argument("Количество функций активации должно совпадать с количеством их производных!");
    }

    if (!activationFuncList.empty() && neuralNetStruct.size() - 1 != activationFuncList.size()) {
        throw std::invalid_argument("Количество функций активации должно совпадать с количеством слоёв!");
    }

    for (size_t i = 0; i < neuralNetStruct.size() - 1; i++) {
        if (activationFuncList.empty()) {
            weights.push_back(Layer(neuralNetStruct[i], neuralNetStruct[i + 1]));
        } else {
            weights.push_back(Layer(neuralNetStruct[i], neuralNetStruct[i + 1],
                activationFuncList[i], activationFuncDerivativeList[i]));
        }
    }
}

void NeuralNetwork::setWeights(std::vector<nnlab::Matrix>& weightsList) {
    if (weights.size() != weightsList.size()) {
        throw std::invalid_argument("Количество матриц весов не совпадает с количеством слоёв!");
    }

    for (size_t i = 0; i < weightsList.size(); i++) {
        if (weightsList[i].cols() != weights[i].weights.cols() ||
            weightsList[i].rows() != weights[i].weights.rows()) {
            throw std::invalid_argument("Размеры матриц весов не совпадают со структурой нейронной сети!");
        }

        weights[i].weights = weightsList[i];
    }
}

std::vector<nnlab::Matrix> NeuralNetwork::getWeights(){
    std::vector<nnlab::Matrix> weightsList;

    for (auto& i : weights) {
        weightsList.push_back(i.weights);
    }
    return weightsList;
}

std::vector<nnlab::Matrix> NeuralNetwork::forward(nnlab::Matrix vector) {
    std::vector<nnlab::Matrix> info;
    info.push_back(vector);

    for (int l = 0; l < weights.size(); l++) {
        vector = weights[l].weights * vector;
        for (size_t i = 0; i < vector.rows(); i++) {
            for (size_t j = 0; j < vector.cols(); j++) {
                vector(i, j) = weights[l].activationFunc(vector(i, j));
            }
        }
        info.push_back(vector);
    }
    return info;
}

std::vector<double> NeuralNetwork::predictProba(std::vector<nnlab::Matrix>& inputData) {
    std::vector<double> prediction(inputData.size());

    for (size_t i = 0; i < inputData.size(); i++) {
        prediction[i] = forward(inputData[i]).back()(0, 0);
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

void NeuralNetwork::train(Dataset& trainData,
    const LossFunction& lossFunc, const LossFunction& lossDerivative, int maxEpochs, double learningRate, double minDelta, int patience) {

    // используем sigmoid на всех слоях, MSE как Loss по умлочанию
    auto trainValues = trainData.first;
    auto trainPoints = trainData.second;
    double prevLoss = 0.0;
    double epochLoss = 0.0;
    int curPatience = 0;

    // проходимся по эпохам
    for (int epo = 0; epo <= maxEpochs; epo++) {
        epochLoss = 0.0;

        // проходимся по точкам из обучающей выборки
        for (size_t i = 0; i < trainPoints.size(); i++) {

            // считаем output(x)
            auto vector = trainValues[i];
            std::vector<nnlab::Matrix> info = forward(vector);

            // считаем delta(x)
            std::vector<nnlab::Matrix> delta;

            // выходной слой
            nnlab::Matrix lastLayer = nnlab::Matrix(weights.back().outputSize, 1);
            for (size_t j = 0; j < weights.back().outputSize; j++) {
                lastLayer(j, 0) = - lossDerivative(info.back()(j, 0), trainPoints[i]) *
                    weights.back().activationFuncDerivative(info.back()(j, 0));
            }
            delta.push_back(lastLayer);

            // скрытые слои
            for (int l = weights.size() - 2; l >= 0; l--) {
                auto tempDelta = nnlab::Matrix(weights[l].outputSize, 1);
                for (size_t j = 0; j < weights[l].outputSize; j++) {
                    double sum = 0.0;
                    for (size_t k = 0; k < weights[l + 1].outputSize; k++) {
                        sum += delta.back()(k, 0) * weights[l + 1].weights(k, j);
                    }
                    tempDelta(j, 0) = weights[l].activationFuncDerivative(info[l + 1](j, 0)) * sum;
                }
                delta.push_back(tempDelta);
            }
            std::reverse(delta.begin(), delta.end());

            // считаем p(w)
            std::vector<nnlab::Matrix> pVectors;

            for (size_t j = 0; j < delta.size(); j++) {
                auto pTemp = nnlab::Matrix(weights[j].outputSize, weights[j].inputSize);
                for (size_t rows = 0; rows < pTemp.rows(); rows++) {
                    for (size_t cols = 0; cols < pTemp.cols(); cols++) {
                        pTemp(rows, cols) = delta[j](rows, 0) * info[j](cols, 0);
                    }
                }
                pVectors.push_back(pTemp);
            }

            // считаем обновленные веса

            for (size_t j = 0; j < weights.size(); j++) {
                weights[j].weights = weights[j].weights + pVectors[j] * learningRate;
            }

            {
                const double MAX_WEIGHT = 2.0;
                for (size_t j = 0; j < weights.size(); j++) {
                    for (size_t r = 0; r < weights[j].weights.rows(); ++r) {
                        for (size_t c = 0; c < weights[j].weights.cols(); ++c) {
                            double& w = weights[j].weights(r, c);
                            w = std::max(-MAX_WEIGHT, std::min(MAX_WEIGHT, w));
                        }
                    }
                }
            }

            epochLoss += lossFunc(info.back()(0, 0), trainPoints[i]);
        }

        // Если разница в Loss между прошлой и нынешней эпохой меньше Epsilon, увеличиваем счётчик
        if (std::abs((prevLoss - epochLoss) / trainPoints.size()) < minDelta) {
            curPatience += 1;
        } else { curPatience = 0; }

        // Если счётчик превысил заданное значение, значит обучение закончено
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


