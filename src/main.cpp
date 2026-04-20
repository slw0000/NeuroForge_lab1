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
     Для запуска тестирования необходимо запустить программу с конфигурацией "--test".
    Для запуска демонстрации нейросети необходимо запустить программу с концигурацией --example
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
        NeuralNetwork default_model = NeuralNetwork();
        NeuralNetwork custom_model = NeuralNetwork(
            {2, 4, 4, 1},
            {nnlab::tanh,  nnlab::tanh, sigmoid},
            {nnlab::tanhDerivative,  nnlab::tanhDerivative, sigmoidDerivative});

        auto trainData = minMaxNormalization(genBinClassifyDataset(1000, 0.6));
        auto testData = minMaxNormalization(genBinClassifyDataset(300, 0.6));

        fileSaveToCSV("data/example.csv", testData.first, testData.second);
        plot("data/example.csv");

        default_model.train(trainData);
        custom_model.train(trainData, bceLoss, bceDerivative, 5000,
            0.01,
            1e-5,
            20);

        auto weights = default_model.getWeights();
        for (Matrix& i: weights) { std::cout << "\nLayer: \n" << i << std::endl; }

        auto test1 = default_model.predict(testData.first);
        auto test2 = custom_model.predict(testData.first);
        auto testProba1 = default_model.predictProba(testData.first);
        auto testProba2 = custom_model.predictProba(testData.first);

        std::cout << "\nProbability test for default model:" << std::endl;
        for (int i = 0; i < test1.size(); i = i + 20) { std::cout << test1[i] << " " << testProba1[i] << std::endl; }

        std::cout << "\nProbability test for custom model:" << std::endl;
        for (int i = 0; i < test2.size(); i = i + 20) { std::cout << test2[i] << " " << testProba2[i] << std::endl; }


        std::cout << "\nAccuracy: \ndefault: " << accuracy(test1, testData.second) <<
            "\ncustom: " << accuracy(test2, testData.second) << std::endl;
        std::cout << "\nRoc-Auc: \ndefault: " << rocAuc(testProba1, testData.second) <<
            "\ncustom: " << rocAuc(testProba2, testData.second) << std::endl;

        auto weights_custom = custom_model.getWeights();
        for (Matrix& i: weights_custom) { std::cout << "\nLayer: \n" << i << std::endl; }
    }
    else{
        return 0;
    }
}
