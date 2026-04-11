#include "../include/utils.h"

#include <random>


double nnlab::genRandomNumber(double min, double max) {
    std::random_device rand_seed;
    static std::mt19937 generate(rand_seed());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(generate);
}

int nnlab::genRandomInt(int min, int max) {
    std::random_device rand_seed;
    static std::mt19937 generate(rand_seed());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generate);
}

std::pair<std::vector<nnlab::Matrix>, std::vector<int>> nnlab::genBinClassifyDataset(int count, double noise,
    double x0_center, double y0_center, double x1_center, double y1_center) {
    if (count <= 0) {
        throw std::invalid_argument("Ошибка генерации датасета: количество элементов должно быть > 0!");
    }

    std::vector<Matrix> cords;
    std::vector<int> label;

    for (int i = 0; i < count; i++) {
        int labelIndex = genRandomInt(0, 1);
        label.push_back(labelIndex);

        double baseX = (labelIndex == 0) ? x0_center : x1_center;
        double baseY = (labelIndex == 0) ? y0_center : y1_center;

        double x = baseX + genRandomNumber(-0.5, 0.5) * noise;
        double y = baseY + genRandomNumber(-0.5, 0.5) * noise;

        Matrix dot = Matrix(2, 1);
        dot(0, 0) = x;
        dot(1, 0) = y;
        cords.push_back(dot);
    }

    return std::make_pair(cords, label);
}

std::pair<std::vector<nnlab::Matrix>, std::vector<int> > nnlab::minMaxNormalization(
    const std::pair<std::vector<Matrix>, std::vector<int> > &data) {

    if (data.first.empty()) { throw std::invalid_argument("Массив координат не может быть пустым!"); }

    double minX, minY, maxX, maxY;
    minX = data.first[0](0, 0);
    minY = data.first[0](1, 0);
    maxX = data.first[0](0, 0);
    maxY = data.first[0](1, 0);

    for (const auto& dot: data.first) {
        minX = std::min(minX, dot(0, 0));
        minY = std::min(minY, dot(1, 0));
        maxX = std::max(maxX, dot(0, 0));
        maxY = std::max(maxY, dot(1, 0));
    }

    auto result = data;
    for (auto& dot: result.first) {
        dot(0, 0) = (maxX != minX) ? (dot(0, 0) - minX) / (maxX - minX) : 0.0;
        dot(1, 0) = (maxY != minY) ? (dot(1, 0) - minY) / (maxY - minY) : 0.0;
    }

    return result;
}

double nnlab::sigmoid(double x) {
    return 1.0 / (1.0 + std::exp(-x));
}

double nnlab::sigmoidDerivative(double sigmoidOutput) {
    return sigmoidOutput * (1.0 - sigmoidOutput);
}

double nnlab::relu(double x) {
    return std::max(0.0, x);
}

double nnlab::reluDerivative(double x) {
    if (x > 0.0) { return 1.0; }
    return 0.0;
}

double nnlab::tanh(double x) {
    return std::tanh(x);
}

double nnlab::tanhDerivative(double tanhOutput) {
    return 1.0 - tanhOutput * tanhOutput;
}

double nnlab::mseLoss(double predictValue, int correctValue) {
    double diff = predictValue - correctValue;
    return 0.5 * (diff * diff);
}

double nnlab::mseDerivative(double predictValue, int correctValue) {
    return predictValue - correctValue;
}

double nnlab::bceLoss(double predictValue, int correctValue) {
    return - (correctValue * std::log(predictValue) + (1 - correctValue) * std::log(1 - predictValue));
}

double nnlab::bceDerivative(double predictValue, int correctValue) {
    return (predictValue - correctValue) / (predictValue * (1 - predictValue));
}

double nnlab::accuracy(const std::vector<int>& predict, const std::vector<int>& correct) {
    double sum = 0;
    for (int i = 0; i < predict.size(); i++) {
        if (predict[i] == correct[i]) {
            sum += 1;
        }
    }

    return sum / static_cast<double>(predict.size());
}

double nnlab::rocAuc(const std::vector<double> &predictProba, const std::vector<int> &correct) {
    std::vector<std::pair<double, int>> data;
    for (size_t i = 0; i < predictProba.size(); i++)
        data.push_back({predictProba[i], correct[i]});

    std::sort(data.begin(), data.end(),
        [] (const auto& a, const auto& b) { return a.first > b.first; });

    int totalPos = 0, totalNeg = 0;
    for (int l : correct) { l == 1 ? totalPos++ : totalNeg++; }

    double rocauc = 0.0;
    double prevFPR = 0.0, prevTPR = 0.0;
    int tp = 0, fp = 0;

    for (const auto& [prob, label] : data) {
        label == 1 ? tp++ : fp++;
        double tpr = static_cast<double>(tp) / totalPos;
        double fpr = static_cast<double>(fp) / totalNeg;

        rocauc += (fpr - prevFPR) * (tpr + prevTPR) / 2.0;

        prevFPR = fpr;
        prevTPR = tpr;
    }

    return rocauc;
}
