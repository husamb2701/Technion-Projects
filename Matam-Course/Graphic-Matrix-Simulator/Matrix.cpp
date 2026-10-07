#include "Matrix.h"
#include <cmath>
#include<iostream>
#include "Utilities.h"

Matrix::Matrix(const Matrix& matrix) : n(matrix.n), m(matrix.m), data(nullptr) {
    if (matrix.data != nullptr) {
        this->data = new int[matrix.n * matrix.m];
        for(int i = 0; i < matrix.n * matrix.m; i++) {
            data[i] = matrix.data[i];
        }
    }
}

Matrix& Matrix::operator=(const Matrix& matrix) {
    if (this == &matrix) {
        return *this;
    }

    delete[] this->data;
    this->n = matrix.n, this->m = matrix.m;
    if (matrix.data != nullptr) {
        this->data = new int[matrix.n * matrix.m];
        for(int i = 0; i < matrix.n * matrix.m; i++) {
            data[i] = matrix.data[i];
        }
    }else {
        this->data = nullptr;
    }
    return *this;
}

int& Matrix::operator()(int i, int j) {
    if (i < 0 || i >= this->n || j < 0 || j >= this->m) {
        exitWithError(MatamErrorType::OutOfBounds);
    }
    return data[i * m + j];
}

int Matrix::operator()(int i, int j) const {
    if (i < 0 || i >= this->n || j < 0 || j >= this->m) {
        exitWithError(MatamErrorType::OutOfBounds);
    }
    return data[i * m + j];
}

std::ostream& operator<<(std::ostream& os, const Matrix& matrix) {
    for (int i = 0; i < matrix.n; ++i) {
        os << "|";
        for (int j = 0; j < matrix.m; ++j) {
            os << matrix(i, j) << "|";
        }
        os << '\n';
    }
    return os;
}

Matrix& Matrix::operator+= (Matrix const& matrix2) {
    if (this->n != matrix2.n || this->m != matrix2.m) {
        exitWithError(MatamErrorType::UnmatchedSizes);
    }
    for (int i = 0; i < this->n * this->m; i++) {
        this->data[i] += matrix2.data[i];
    }
    return *this;
}

Matrix& Matrix::operator-= (Matrix const& matrix2) {
    if (this->n != matrix2.n || this->m != matrix2.m) {
        exitWithError(MatamErrorType::UnmatchedSizes);
    }
    for (int i = 0; i < (this->m * this->n); i++) {
            this->data[i] -= matrix2.data[i];
    }
    return *this;
}

Matrix& Matrix::operator*= (Matrix const& matrix2) {
    if (this->m != matrix2.n) {
        exitWithError(MatamErrorType::UnmatchedSizes);
    }
    auto* temp = new int[this->n * matrix2.m]();
    for (int i = 0; i < this->n; i++) {
        for (int j = 0; j < matrix2.m; j++) {
            for (int k = 0; k < this->m; k++) {
                temp[i * matrix2.m + j] +=
                    this->data[i * this->m + k] * matrix2.data[k * matrix2.m + j];
            }
        }
    }
    this->m = matrix2.m;
    delete[] data;
    this->data = temp;
    return *this;
}

Matrix& Matrix::operator*= (int const num) {
    if (this->data != nullptr) {
        for (int i = 0; i < this->n * this->m; i++) {
            this->data[i] *= num;
        }
    }
    return *this;
}

Matrix Matrix::operator+ (Matrix const& matrix2) const {
    if (this->n != matrix2.n || this->m != matrix2.m) {
        exitWithError(MatamErrorType::UnmatchedSizes);
    }
    Matrix result(*this);
    result += matrix2;
    return result;
}

Matrix Matrix::operator- (Matrix const& matrix2) const {
    if (this->n != matrix2.n || this->m != matrix2.m) {
        exitWithError(MatamErrorType::UnmatchedSizes);
    }
    Matrix result(*this);
    result -= matrix2;
    return result;
}

Matrix Matrix::operator-() const {
    Matrix temp(*this);
    temp *= -1;
    return temp;
}

Matrix Matrix::operator* (Matrix const& matrix2) const {
    if (this->m != matrix2.n) {
        exitWithError(MatamErrorType::UnmatchedSizes);
    }
    Matrix result(*this);
    result *= matrix2;
    return result;
}

Matrix Matrix::operator* (int const num) const {
    Matrix result(*this);
    result *= num;
    return result;
}

bool Matrix::operator== (Matrix const& matrix2) const {
    if (m != matrix2.m || n != matrix2.n) {
        exitWithError(MatamErrorType::UnmatchedSizes);
    }
        for (int i = 0 ; i < m * n; i++) {
            if (data[i] != matrix2.data[i]) return false;
        }
    return true;
}

bool Matrix::operator!= (Matrix const& matrix2) const {
    return !(*this == matrix2);
}

Matrix Matrix::rotateClockwise() const {
    Matrix rotatedMatrix(m, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            rotatedMatrix.data[j * n + (n - 1 - i)] = data[i * m + j];
        }
    }
    return rotatedMatrix;
}

Matrix Matrix::rotateCounterClockwise() const {
    Matrix rotatedMatrix(m, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            rotatedMatrix.data[(m - 1 - j) * n + i] = data[i * m + j];
        }
    }
    return rotatedMatrix;
}

Matrix Matrix::transpose() const {
    Matrix transposedMatrix(m, n);
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            transposedMatrix.data[j * n + i] = data[i * m + j];
        }
    }
    return transposedMatrix;
}

double Matrix::CalcFrobeniusNorm(const Matrix& matrix){
    double sum = 0;
    for (int i = 0; i < matrix.n * matrix.m; i++) {
        sum += matrix.data[i] * matrix.data[i];
    }
    return sqrt(sum);
}

int Matrix::CalcDeterminant(const Matrix& mat){
    if (mat.m != mat.n) {
        exitWithError(MatamErrorType::NotSquareMatrix);
    }
    if (mat.m == 1) {
        return mat.data[0];
    }
    if (mat.m == 2) {
        return mat.data(0, 0) * mat.data (1,1) - mat.data (0,1) * mat.data(1,0);
    }
    int det = 0;
    for (int col = 0; col < mat.m; col++) {
        Matrix minor = MatrixMinor(mat, col);
        int value = mat (0, col) * CalcDeterminant(minor);
        int sign = col % 2 == 0 ? 1 : -1;
        det += sign * value;
    }
    return det;
}

Matrix Matrix::MatrixMinor(const Matrix& matrix,const int index) {
    Matrix minorMatrix(matrix.n - 1,matrix.m - 1, 0);
    int minorRow = 0;
    for (int i = 1; i < matrix.n; i++) {
        int minorCol = 0;
        for (int j = 0; j < matrix.m; j++) {
            if (j == index) {
            continue;
            }
            minorMatrix(minorRow,minorCol++) = matrix(i,j);
        }
        minorRow++;
    }
    return minorMatrix;
}

Matrix& operator*= (int num, Matrix& matrix1) {
    matrix1 *= num;
    return matrix1;
}

Matrix operator* (const int num,const Matrix& matrix2) {
    Matrix temp(matrix2);
    temp *= num;
    return temp;
}
