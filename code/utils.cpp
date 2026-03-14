#include "utils.h"

#include <random>



double nnlab::genRandomNumber(double min = -1.0, double max = 1.0) {
    std::random_device rand_seed;
    std::mt19937 generate(rand_seed());
    std::uniform_real_distribution<double> dist(min, max);
    return dist(generate);
}

int nnlab::genRandomInt(int min = -1, int max = 1) {
    std::random_device rand_seed;
    std::mt19937 generate(rand_seed());
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
    Matrix dot = Matrix(2, 1);

    for (int i = 0; i < count; i++) {
        int labelIndex = genRandomInt(0, 1);
        label.push_back(labelIndex);

        double baseX = (labelIndex == 0) ? x0_center : x1_center;
        double baseY = (labelIndex == 0) ? y0_center : y1_center;

        double x = baseX + genRandomNumber(-0.5, 0.5) * noise;
        double y = baseY + genRandomNumber(-0.5, 0.5) * noise;

        dot(0, 0) = x;
        dot(1, 0) = y;
        cords.push_back(dot);
    }

    return std::make_pair(cords, label);
}



