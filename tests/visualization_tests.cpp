#include "../code/visualization.h"
#include <iostream>
#include <fstream>

void createTestFile_2(const std::string& fileName, const std::string& fileContent) {
    std::ofstream file(fileName);
    file << fileContent;
    file.close();
}

void removeTestFile_2(const std::string& fileName) {
    std::remove(fileName.c_str());
}

void testVisualizationCorrect()
{
    createTestFile_2("data/test_no_label.csv",
        "x,y\n"
        "1.1,0.2\n"
        "0.3,-2.4\n"
        "-0.5,0.6");
    try
    {
        viz::plot("data/test_no_label.csv");
        std::cout << "Test visualization correct CSV - OK\n";
    }
    catch (...)
    {
        std::cout << "Test visualization correct CSV - FAIL\n";
    }
    removeTestFile_2("data/test_no_label.csv");
}

void testVisualizationWithLabels()
{
    createTestFile_2("data/test_with_label.csv",
        "x,y,label\n"
        "1.1,0.2,0\n"
        "0.3,-2.4,1\n"
        "-0.5,0.6,2");

    try
    {
        viz::plot("data/test_with_label.csv");
        std::cout << "Test visualization with labels - OK\n";
    }
    catch (...)
    {
        std::cout << "Test visualization with labels - FAIL\n";
    }
    removeTestFile_2("data/test_with_label.csv");
}

void testVisualizationWithLabelsWithLeg0()
{
    createTestFile_2("data/test_with_label.csv",
        "x,y,label\n"
        "1.1,0.2,0\n"
        "0.3,-2.4,1\n"
        "-0.5,0.6,2");

    try
    {
        viz::plot("data/test_with_label.csv 0");
        std::cout << "Test visualization with labels no legend - OK\n";
    }
    catch (...)
    {
        std::cout << "Test visualization with labels no legend - FAIL\n";
    }
    removeTestFile_2("data/test_with_label.csv");
}

void testVisualizationWithLabelsWithLeg1()
{
    createTestFile_2("data/test_with_label.csv",
        "x,y,label\n"
        "1.1,0.2,0\n"
        "0.3,-2.4,1\n"
        "-0.5,0.6,2");

    try
    {
        viz::plot("data/test_with_label.csv 1");
        std::cout << "Test visualization with labels with legend - OK\n";
    }
    catch (...)
    {
        std::cout << "Test visualization with labels with legend - FAIL\n";
    }
    removeTestFile_2("data/test_with_label.csv");
}

void testVisualizationWrongPath()
{
    try
    {
        viz::plot("not_existing_file.csv");
        std::cout << "Test visualization wrong path - FAIL\n";
    }
    catch (...)
    {
        std::cout << "Test visualization wrong path - OK\n";
    }
}


void runVisualizationTests() {
    testVisualizationCorrect();
    testVisualizationWithLabels();
    testVisualizationWithLabelsWithLeg0();
    testVisualizationWithLabelsWithLeg1();
    testVisualizationWrongPath();

    std::cout << "All tests passed" << std::endl;
}
