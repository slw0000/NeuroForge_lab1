// #include <windows.h> /* это только для VScode на Windows */

#include <vector>

#include "../include/file_import.h"
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
     Для запуска показа примера модели необходимо запустить программу с конфигурацией "--example".
    */

    bool runTests = false;
    bool exampleShow = false;
    for (int i = 1; i < argc; i++) {
        if (std::string(argv[i]) == "--test") {
            runTests = true;
            break;
        }
        else if (std::string(argv[i]) == "--example") {
            exampleShow = true;
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
    else if (exampleShow) {
        auto [trainCoords, trainLabels] = genBinClassifyDataset(1000, 0.8,
        0.4, 0.4,
        0.6, 0.6
        );
        auto [testCoords, testLabels]   = genBinClassifyDataset(200, 0.8);

        auto trainData = minMaxNormalization({trainCoords, trainLabels});
        auto testData  = minMaxNormalization({testCoords, testLabels});

        fileSaveToCSV("data/example_test.csv", testData.first, testData.second);

        NeuralNetwork default_model = NeuralNetwork();
        NeuralNetwork custom_model = NeuralNetwork({2, 4, 8, 4, 1},
            {relu, relu, relu, sigmoid},
            {reluDerivative, reluDerivative, reluDerivative, sigmoidDerivative});


        default_model.train(trainData);
        custom_model.train(trainData,
            bceLoss, bceDerivative,
            0.01,
            5000,
            1e-4,
            15
        );

        // === DEBUG: проверка весов ===
        auto weights = custom_model.getWeights();
        std::cout << "\n[DEBUG] Weight statistics:" << std::endl;
        for (size_t l = 0; l < weights.size(); ++l) {
            double maxW = 0, sumW = 0;
            for (size_t i = 0; i < weights[l].rows(); ++i)
                for (size_t j = 0; j < weights[l].cols(); ++j) {
                    double w = std::abs(weights[l](i, j));
                    maxW = std::max(maxW, w);
                    sumW += w;
                }
            double avgW = sumW / (weights[l].rows() * weights[l].cols());
            std::cout << "  Layer " << l << ": max|w|=" << maxW
                      << ", avg|w|=" << avgW << std::endl;
        }

        auto predictions_default = default_model.predict(testData.first);
        auto predictions_custom  = custom_model.predict(testData.first);

        auto probabilities_default = default_model.predictProba(testData.first);
        auto probabilities_custom  = custom_model.predictProba(testData.first);

        double acc_default = accuracy(predictions_default, testData.second);
        double acc_simple  = accuracy(predictions_custom, testData.second);

        double auc_default = rocAuc(probabilities_default, testData.second);
        double auc_simple  = rocAuc(probabilities_custom, testData.second);

        std::cout << "\n RESULTS" << std::endl;

        std::cout << "\n Default model (2-4-4-1, MSE):" << std::endl;
        std::cout << "   Accuracy: " << acc_default << std::endl;
        std::cout << "   ROC-AUC:  " << auc_default << std::endl;

        std::cout << "\n Custom model (2-4-8-4-1, BCE, relu):" << std::endl;
        std::cout << "   Accuracy: " << acc_simple << std::endl;
        std::cout << "   ROC-AUC:  " << auc_simple << std::endl;

        std::cout << "Example of predictions (10 first points):" << std::endl;
        std::cout << "№\tPrediction\tProbability\tTrue class" << std::endl;
        for (int i = 0; i < 10; ++i) {
            std::cout << i << "\t"
                      << predictions_custom[i] << "\t\t"
                      << probabilities_custom[i] << "\t\t"
                      << testData.second[i] << std::endl;
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

