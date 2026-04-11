// #include <windows.h> /* это только для VScode на Windows */

#include <vector>

#include "file_import.h"
#include "../include/neural_network.h"
#include "../include/visualization.h"

void runAllMatrixTests();
void runAllFileImportTests();
void runVisualizationTests();

using namespace nnlab;

int main(int argc, char* argv[]) {

    // SetConsoleOutputCP(CP_UTF8); /* это только для VScode на Windows */
    // SetConsoleCP(CP_UTF8);       /* это только для VScode на Windows */

    /*
     Для запуска демонстрации работы класса матриц и утилиты работы с файлами, надо просто запустить программу.
     Для запуска тестирования необходимо запустить программу с конфигурацией "--test".
    */

    bool runTests = false;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--test") {
            runTests = true;
            break;
        }
    }

    if (runTests) {
        std::cout << "Matrix tests: \n" << std::endl;
        try {
            runAllMatrixTests();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        } catch (...) {
            std::cerr << "Unknown error" << std::endl;
            return 1;
        }

        std::cout << "\nFile import tests: \n" << std::endl;
        try {
            runAllFileImportTests();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        } catch (...) {
            std::cerr << "Unknown error" << std::endl;
            return 1;
        }
        std::cout << "\nVisualization tests: \n" << std::endl;
        try {
            runVisualizationTests();
        } catch (const std::exception& e) {
            std::cerr << "Error: " << e.what() << std::endl;
            return 1;
        } catch (...) {
            std::cerr << "Unknown error" << std::endl;
            return 1;
        }
    }
    else{

        NeuralNetwork net1 = NeuralNetwork(), net2 = NeuralNetwork();

        auto trainData = minMaxNormalization(genBinClassifyDataset(1000, 0.4));
        auto testData  = minMaxNormalization(genBinClassifyDataset(200, 0.6));

        fileSaveToCSV("data/testData.csv", testData.first, testData.second);
        plot("data/testData.csv");


        net1.train(trainData);
        net2.train(trainData, bceLoss, bceDerivative);

        // auto weights = net1.getWeights();
        // for (Matrix& i: weights) { std::cout << "\nLayer: \n" << i << std::endl; }

        auto test1 = net1.predict(testData.first);
        auto test2 = net2.predict(testData.first);
        auto testProba1 = net1.predictProba(testData.first);
        auto testProba2 = net2.predictProba(testData.first);

        std::cout << "\nProbability test for net1:" << std::endl;
        for (int i = 0; i < test1.size(); i = i + 20) { std::cout << test1[i] << " " << testProba1[i] << std::endl; }

        std::cout << "\nProbability test for net2:" << std::endl;
        for (int i = 0; i < test2.size(); i = i + 20) { std::cout << test2[i] << " " << testProba2[i] << std::endl; }


        std::cout << "\nAccuracy: \nnet1: " << accuracy(test1, testData.second) <<
            "\nnet2: " << accuracy(test2, testData.second) << std::endl;
        std::cout << "\nRoc-Auc: \nnet1: " << rocAuc(testProba1, testData.second) <<
            "\nnet2: " << rocAuc(testProba2, testData.second) << std::endl;


        return 0;
    }
}

