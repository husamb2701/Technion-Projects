#pragma once
#include <iostream>

class Matrix {
public:
    Matrix() : n(0), m(0), data(nullptr) {};
    Matrix(int n, int m):n(n), m(m), data(new int[n*m]()) {};
    Matrix(int n, int m, int num) : n(n), m(m), data(new int [n*m]) {
        for (int i = 0; i < n*m; i++) {
            data[i] = num;
        }
    };
    Matrix(const Matrix&);
    Matrix& operator=(const Matrix& matrix);
    ~Matrix() {
        delete [] data;
    }
    int& operator()(int i, int j);
    int operator()(int i, int j) const;

    friend std::ostream& operator<<(std::ostream& os, const Matrix& matrix);

    Matrix& operator+= (Matrix const& matrix2);
    Matrix& operator-= (Matrix const& matrix2);
    Matrix& operator*= (Matrix const& matrix2);
    Matrix& operator*= (int num);

    Matrix operator+ (Matrix const& matrix2)const;
    Matrix operator- (Matrix const& matrix2)const;
    Matrix operator- ()const;
    Matrix operator* (Matrix const& matrix2) const;
    Matrix operator* (int num) const;

    bool operator== (Matrix const& matrix2) const;
    bool operator!= (Matrix const& matrix2) const;

    Matrix rotateClockwise() const;
    Matrix rotateCounterClockwise() const;
    Matrix transpose() const;

    static double CalcFrobeniusNorm(const Matrix& matrix);
    static int CalcDeterminant(const Matrix& matrix);
    static Matrix MatrixMinor(const Matrix& matrix, int index);


private:
    int n = 0;
    int m = 0;
    int* data = nullptr;
};

Matrix operator* (int num, Matrix& matrix2);
