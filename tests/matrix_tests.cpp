#include <cassert>
#include <iostream>
#include <cmath>
#include "matrix_realization.h"

void assertDoubleEquals(double actual, double expected, double tolerance= 0.000) {
    if (std::abs(actual - expected) > tolerance) {
        std::cerr << "Mistake, expected: " << expected
        << ", actual: " << actual << std::endl;
    }
}

void testConstructor() {
    Matrix m(4,10, 10);
    assert(m.rows() == 4 && m.cols() == 10);
    Matrix m2(99,2, 42);
    assert(m2.rows() == 99 && m2.cols() == 2);
    std::cout << "Test constructor - OK" << std::endl;
}

void testElementAccess() {
    Matrix m(4,2, 10.3);
    assertDoubleEquals(m(2,1) , 10.3);
    assertDoubleEquals(m(0,0), 10.3);
    assertDoubleEquals(m(0,1), 10.3);
    m(3,1) = 42;
    assert(m(3,1) == 42);
    std::cout << "Test element_access - OK" << std::endl;
}

void testMatriceAdd() {
    Matrix a(3,2,1.5);
    Matrix b(3,2,42.5);
    Matrix result = a + b;
    assert(result.rows() == 3 && result.cols() == 2);
    assertDoubleEquals(result(0,0), 44);
    assertDoubleEquals(result(2,1), 44);
    std::cout << "Test matrice_add - OK" << std::endl;
}

void testMatriceSub() {
    Matrix a(3,2,1.5);
    Matrix b(3,2,43.92);
    Matrix result = b - a;
    assert(result.rows() == 3 && result.cols() == 2);
    assertDoubleEquals(result(0,0), 42.42);
    assertDoubleEquals(result(2,1), 42.42);
    std::cout << "Test matrice_sub - OK" << std::endl;
}

void testMatriceMul() {
    Matrix a(3,2,1.5);
    Matrix b(2,3,12);
    Matrix result = a * b;
    assert(result.rows() == 3 && result.cols() == 3);
    assertDoubleEquals(result(0,0), 36);
    assertDoubleEquals(result(2,2), 36);
    std::cout << "Test matrice_mul - OK" << std::endl;
}

void testMatrixMulScalar() {
    Matrix a(3,2,1.5);
    Matrix result = a * 2.0;
    assert(result.rows() == 3 && result.cols() == 2);
    assertDoubleEquals(result(0,0), 3.0);
    assertDoubleEquals(result(2,1), 3.0);
    std::cout << "Test matrix_mul_scalar - OK" << std::endl;
}

void testMatrixTranspose() {
    Matrix a(5, 4, 2.4);
    a(0, 3) = 1.0;
    a = a.transpose();
    assert(a.rows() == 4 && a.cols() == 5);
    assertDoubleEquals(a(3,0), 1.0);
    assertDoubleEquals(a(0,3), 2.4);
    std::cout << "Test matrix_transpose - OK" << std::endl;
}

void testIndexOutOfRange() {
    bool caught = false;
    try {
        Matrix m(4, 2, 21.21);
        m(42, 42);
    } catch (const std::out_of_range&) {
        caught = true;
    }
    assert(caught);
    std::cout << "Test Out of index - OK" << std::endl;
}

void testSizeException() {
    bool caught = false;
    try {
        Matrix m(0, 0, 0);
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    std::cout << "Test size exception - OK" << std::endl;
}

void testMatrixAddSizeException() {
    bool caught = false;
    try {
        Matrix a(3,2, 1.5);
        Matrix b(5,2, 42.5);
        Matrix result = a + b;
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    std::cout << "Test matrix add size exception - OK" << std::endl;
}

void testMatrixSubSizeException() {
    bool caught = false;
    try {
        Matrix a(3,2, 1.5);
        Matrix b(5,2, 42.5);
        Matrix result = a - b;
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    std::cout << "Test matrix sub size exception - OK" << std::endl;
}

void testMatrixMulSizeException() {
    bool caught = false;
    try {
        Matrix a(3,2, 1.5);
        Matrix b(5,2, 43.92);
        Matrix result = a * b;
    } catch (const std::invalid_argument&) {
        caught = true;
    }
    assert(caught);
    std::cout << "Test matrix mul size exception - OK" << std::endl;
}

void runAllMatrixTests() {
    testConstructor();
    testElementAccess();
    testMatriceAdd();
    testMatriceSub();
    testMatriceMul();
    testMatrixMulScalar();
    testMatrixTranspose();
    testIndexOutOfRange();
    testSizeException();
    testMatrixAddSizeException();
    testMatrixSubSizeException();
    testMatrixMulSizeException();
    std::cout << "All tests passed" << std::endl;
}