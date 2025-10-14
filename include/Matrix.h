#ifndef MATRIX_H
#define MATRIX_H

#include <vector>
#include <cstddef>
#include <string>
#include "Vector.h"  // forward declaration or full include if needed
using namespace std;

class Matrix
{
public:

    // Constructors
    Matrix();
    Matrix(std::initializer_list<double> values);

    // Operators to fetch the values at a particular position
    double& operator()(size_t row, size_t col);
    const double& operator()(size_t row, size_t col) const;

    // Function to return value 0-9
    double at(size_t index) const;
    size_t size() const;

    // Function to return the string to display the matrix
    std::string display() const;

    // Operators for matrix multiplication
    Vector operator*(const Vector& vec) const;
    Matrix operator*(const Matrix& other) const;
    Matrix operator+(const Matrix& other) const;
    Matrix& operator+=(const Matrix& other);

    // Function to return characteresic values of matrices
    double determinant() const;
    Matrix transpose() const;
    Matrix adjoint() const;
    Matrix inverse() const;

private:

    void check_bounds(size_t row, size_t col) const;

    vector<double> data;
};

#endif // MATRIX_H

