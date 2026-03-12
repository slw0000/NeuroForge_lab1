#include <cassert>
#include <iostream>
#include <fstream>

#include "../code/file_import.h"
#include "../code/matrix_realization.h"

void createTestFile(const std::string& fileName, const std::string& fileContent) {
    std::ofstream file(fileName);
    file << fileContent;
    file.close();
}

void removeTestFile(const std::string& fileName) {
    std::remove(fileName.c_str());
}

void testFileImportRawCorrect() {
    createTestFile("data/test_raw.csv",
        "x, y, label\n"
        "1.1, 0.2, 0\n"
        "0.3, -2.4, 1\n"
        "-0.5, 0.6, 8");

    auto data = fileImportRaw("data/test_raw.csv");

    assert(data.size() == 3);
    assert(data[0].size() == 3);
    assert(data[0][0] == 1.1);
    assert(data[0][1] == 0.2);

    removeTestFile("data/test_raw.csv");
    std::cout << "Test vector<vector<>> fileImportRaw - OK" << std::endl;
}

void testFileImportRawComments() {
    createTestFile("data/test_comments.csv",
        "x, y\n"
        "# comment №1\n"
        "1.0, 2.0\n"
        "# another comment\n"
        "3.0, 4.0");

    auto data = fileImportRaw("data/test_comments.csv");

    assert(data.size() == 2);
    assert(data[0][0] == 1.0);
    assert(data[1][0] == 3.0);

    removeTestFile("data/test_comments.csv");
    std::cout << "Test fileImportRaw with comments - OK" << std::endl;
}

void testFileImportRawNoFile() {
    bool caught = false;
    try {
        fileImportRaw("data/nonexistent.csv");
    } catch (const std::runtime_error&) {
        caught = true;
    }

    assert(caught);
    std::cout << "Test fileImportRaw no file - OK" << std::endl;
}

void testFileImportRawInvalidFormat() {
    createTestFile("data/test_invalid.csv",
        "x, y\n"
        "1.0, 2.0\n"
        "abc, def");

    bool caught = false;
    try {
        fileImportRaw("data/test_invalid.csv");
    } catch (const std::invalid_argument&) {
        caught = true;
    }

    removeTestFile("data/test_invalid.csv");

    assert(caught);
    std::cout << "Test fileImportRaw invalid format - OK" << std::endl;
}

void testFileImportMatrixRawCorrect() {
    createTestFile("data/test_matrix.csv",
        "x, y\n"
        "0.1, 0.2\n"
        "0.3, 0.4\n"
        "0.5, 0.6");

    auto data = fileImportMatrixRaw("data/test_matrix.csv");

    assert(data.size() == 3);
    assert(data[0].rows() == 2 && data[0].cols() == 1);
    assert(data[0](0, 0) == 0.1);
    assert(data[0](1, 0) == 0.2);

    removeTestFile("data/test_matrix.csv");
    std::cout << "Test vector<Matrix> fileImportMatrxiRaw - OK" << std::endl;
}

void testFileImportMatrixRawWrongColumnCount() {
    createTestFile("data/test_wrong_cols.csv",
        "x, y\n"
        "0.1, 0.2\n"
        "0.3, 0.4, 0.5");

    bool caught = false;
    try {
        fileImportMatrixRaw("data/test_wrong_cols.csv");
    } catch (const std::invalid_argument&) {
        caught = true;
    } catch (...) {
        caught = true;
    }

    removeTestFile("data/test_wrong_cols.csv");
    assert(caught);

    std::cout << "Test fileImportMatrixRaw with wrong amount of columns - OK" << std::endl;
}

void testFileImportMatrixLabelCorrect() {
    createTestFile("data/test_labeled.csv",
        "x, y, label\n"
        "-2.1, 3.2, 4\n"
        "9.3, 3.4, 1\n"
        "-4.5, 0.6, 2");

    auto data = fileImportMatrixLabel("data/test_labeled.csv");

    assert(data.first.size() == 3);
    assert(data.first[0].rows() == 2);
    assert(data.second[0] == 4);
    assert(data.second[1] == 1);

    removeTestFile("data/test_labeled.csv");
    std::cout << "Test fileImportMatrixLabel correct work - OK" << std::endl;
}

void testFileImportMatrixLabelNonIntegerLabel() {
    createTestFile("data/test_float_label.csv",
        "x, y, label\n"
        "0.1, 0.2, 0\n"
        "0.3, 0.4, 0.5");

    bool caught = false;
    try {
        fileImportMatrixLabel("data/test_float_label.csv");
    } catch (const std::invalid_argument&) {
        caught = true;
    } catch (...) {
        caught = true;
    }

    removeTestFile("data/test_float_label.csv");
    assert(caught);
    std::cout << "Test fileImportMatrixLabel wit non int label - OK" << std::endl;
}

void testFileImportMatrixLabelWrongColumnCount() {
    createTestFile("data/test_wrong_cols2.csv",
        "x, y, label\n"
        "0.1, 0.2\n"
        "0.3, 0.4, 1");

    bool caught = false;
    try {
        fileImportMatrixLabel("data/test_wrong_cols2.csv");
    } catch (...) {
        caught = true;
    }

    removeTestFile("data/test_wrong_cols2.csv");
    assert(caught);

    std::cout << "Test fileImportMatrixLable w wrong amount of columns - OK" << std::endl;
}

void testFileSaveToCSVWithoutLabels() {
    Matrix p1(2, 1); p1(0,0) = 1.3; p1(1,0) = -2.4;
    Matrix p2(2, 1); p2(0,0) = 3.2; p2(1,0) = 4.0;
    std::vector<Matrix> cords = {p1, p2};

    fileSaveToCSV("data/test_save_no_labels.csv", cords);

    std::ifstream file("data/test_save_no_labels.csv");
    assert(file.is_open());

    std::string line;
    std::getline(file, line);
    assert(line.find("x,y") != std::string::npos);

    std::getline(file, line);
    assert(line.find("1.3") != std::string::npos);
    assert(line.find("-2.4") != std::string::npos);

    file.close();
    removeTestFile("data/test_save_no_labels.csv");

    std::cout << "Test fileSaveToCSV no labels - OK" << std::endl;
}

void testFileSaveToCSVWithLabels() {
    Matrix p1(2, 1); p1(0,0) = 0.5; p1(1,0) = 0.6;
    Matrix p2(2, 1); p2(0,0) = 0.7; p2(1,0) = 0.8;
    std::vector<Matrix> cords = {p1, p2};
    std::vector<int> labels = {0, 1};

    fileSaveToCSV("data/test_save_with_labels.csv", cords, labels);

    std::ifstream file("data/test_save_with_labels.csv");
    assert(file.is_open());

    std::string line;
    std::getline(file, line);
    assert(line.find("x,y,label") != std::string::npos);

    std::getline(file, line);
    assert(line.find("0.5") != std::string::npos);
    assert(line.find("0.6") != std::string::npos);
    assert(line.find(",0") != std::string::npos);

    std::getline(file, line);
    assert(line.find(",1") != std::string::npos);

    file.close();
    removeTestFile("data/test_save_with_labels.csv");

    std::cout << "Test fileSaveToCSV with labels - OK" << std::endl;
}

void testFileSaveToCSVEmptyCords() {
    std::vector<Matrix> cords;
    std::vector<int> labels;

    bool caught = false;
    try {
        fileSaveToCSV("data/test_empty.csv", cords);
    } catch (const std::invalid_argument&) {
        caught = true;
    } catch (...) {
        caught = true;
    }

    assert(caught);
    std::cout << "Test fileSaveToCSV empty cords - OK" << std::endl;
}

void testFileSaveToCSVWrongSize() {
    Matrix p1(2, 1); p1(0,0) = 0.5; p1(1,0) = 0.6;
    Matrix p2(2, 1); p2(0,0) = 0.7; p2(1,0) = 0.8;
    std::vector<Matrix> cords = {p1, p2};
    std::vector<int> labels = {0, 1, 2, 3};

    bool caught = false;
    try {
        fileSaveToCSV("data/test_save_with_wrong_size.csv", cords, labels);
    } catch (const std::invalid_argument&) {
        caught = true;
    } catch (...) {
        caught = true;
    }

    assert(caught);
    std::cout << "Test fileSaveToCSV wrong size - OK" << std::endl;
}

void runAllFileImportTests() {
    testFileImportRawCorrect();
    testFileImportRawComments();
    testFileImportRawInvalidFormat();
    testFileImportRawNoFile();
    testFileImportMatrixRawCorrect();
    testFileImportMatrixRawWrongColumnCount();
    testFileImportMatrixLabelCorrect();
    testFileImportMatrixLabelNonIntegerLabel();
    testFileImportMatrixLabelWrongColumnCount();
    testFileSaveToCSVWithoutLabels();
    testFileSaveToCSVWithLabels();
    testFileSaveToCSVEmptyCords();
    testFileSaveToCSVWrongSize();


    std::cout << "All tests passed" << std::endl;
}