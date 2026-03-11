#pragma once

#include <vector>
#include <iostream>

class Matrix {
private:
    size_t rows_;
    size_t cols_;
    std::vector<double> data_;

public:
    Matrix(size_t rows, size_t cols, double value = 0.0);
    Matrix(const Matrix& other) = default;
    Matrix& operator=(const Matrix& other) = default;


    size_t rows() const { return rows_; }
    size_t cols() const { return cols_; }


    double& operator()(size_t i, size_t j);
    const double& operator()(size_t i, size_t j) const;

    Matrix operator+(const Matrix& other) const;
    Matrix operator-(const Matrix& other) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator*(double scalar) const;
    Matrix transpose() const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix& m);
};