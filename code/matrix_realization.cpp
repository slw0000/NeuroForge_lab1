#include "matrix_realization.h"

#include <iostream>
#include <vector>
#include <iomanip>
#include <stdexcept>

Matrix::Matrix(size_t rows, size_t cols, double value)
    : rows_(rows), cols_(cols), data_(rows * cols, value){
    if (rows == 0 || cols == 0) {
        throw std::invalid_argument("Matrix size cannot be zero");
    }
}

double& Matrix::operator()(size_t i, size_t j) {
    if (i >= rows_ || j >= cols_) {
        throw std::out_of_range("Matrix index out of range");
    }
    return data_[i * cols_ + j];
}

const double& Matrix::operator()(size_t i, size_t j) const {
    if (i >= rows_ || j >= cols_) {
        throw std::out_of_range("Matrix index out of range");
    }
    return data_[i * cols_ + j];
}

Matrix Matrix::operator+(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix size does not match");
    }
    Matrix result(rows_, cols_, 0.0);
    for (size_t i = 0; i < rows_ * cols_; i++) {
        result.data_[i]= data_[i] + other.data_[i];
    }
    return result;
}

Matrix Matrix::operator-(const Matrix& other) const {
    if (rows_ != other.rows_ || cols_ != other.cols_) {
        throw std::invalid_argument("Matrix size does not match");
    }
    Matrix result(rows_, cols_,0.0);
    for (size_t i = 0; i < rows_ * cols_; i++) {
        result.data_[i]= data_[i] - other.data_[i];
    }
    return result;
}

Matrix Matrix::operator*(const Matrix& other) const {
    if (cols_ != other.rows_) {
        throw std::invalid_argument("Matrices can't be multiply");
    }
    Matrix result(rows_, other.cols_, 0.0);
    for (size_t i = 0; i < rows_; i++) {
        for (size_t j = 0; j < other.cols_; j++) {
            for (size_t k = 0; k < cols_; k++) {
                result(i, j) += (*this)(i, k) * other(k, j);
            }
        }
    }
    return result;
}

Matrix Matrix::operator*(double scalar) const {  // ← Просто double, без const&
    Matrix result(rows_, cols_, 0.0);
    for (size_t i = 0; i < data_.size(); ++i) {
        result.data_[i] = data_[i] * scalar;
    }
    return result;
}

std::ostream& operator<<(std::ostream& os, const Matrix& m) {
    for (size_t i = 0; i < m.rows_; i++) {
        for (size_t j = 0; j < m.cols_; j++) {
            os << std::setw(10) << std::fixed << std::setprecision(2) << m(i, j) << " ";
        }
        os << "\n";
    }
    return os;
}


