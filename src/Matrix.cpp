#include "Matrix.h"
#include <stdexcept>
#include <sstream>
#include <iomanip>
#include <cmath>
#include "Vector.h"

using namespace std;


Matrix::Matrix()
    : data(9, 0.0)
{}


Matrix::Matrix(initializer_list<double> values)
    : data(values)
{
    if (values.size() != 9)
        throw invalid_argument("Matrix must be initialized with 9 elements");
}


double& Matrix::operator()(size_t row, size_t col)
{
    check_bounds(row, col);
    return data[row * 3 + col];
}


const double& Matrix::operator()(size_t row, size_t col) const
{
    check_bounds(row, col);
    return data[row * 3 + col];
}


double Matrix::at(size_t index) const
{
    if (index >= 9)
        throw out_of_range("Index out of range");

    return data[index];
}


size_t Matrix::size() const
{
    return 9;
}


void Matrix::check_bounds(size_t row, size_t col) const
{
    if (row >= 3 || col >= 3)
        throw out_of_range("Matrix indices out of bounds");
}


std::string Matrix::display() const
{
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2);

    for (size_t row = 0; row < 3; ++row)
    {
        oss << "[";
        for (size_t col = 0; col < 3; ++col)
            oss << std::setw(9) << (*this)(row, col);
        oss << " ]\n";
    }

    return oss.str();
}


Vector Matrix::operator*(const Vector& vec) const
{
    return Vector{
        data[0] * vec[0] + data[1] * vec[1] + data[2] * vec[2],
        data[3] * vec[0] + data[4] * vec[1] + data[5] * vec[2],
        data[6] * vec[0] + data[7] * vec[1] + data[8] * vec[2]
    };
}


Matrix Matrix::operator*(const Matrix& other) const
{
    Matrix result;

    for (int row = 0; row < 3; ++row)
        for (int col = 0; col < 3; ++col)
        {
            double sum = 0.0;
            for (int k = 0; k < 3; ++k)
                sum += data[3 * row + k] * other.data[3 * k + col];

            result.data[3 * row + col] = sum;
        }

    return result;
}


Matrix Matrix::operator+(const Matrix& other) const
{
    Matrix result;
    for (size_t i = 0; i < 9; ++i)
        result.data[i] = (*this).data[i] + other.data[i];

    return result;
}


Matrix& Matrix::operator+=(const Matrix& other)
{
    for (size_t i = 0; i < 9; ++i)
        (*this).data[i] += other.data[i];

    return *this;
}


double Matrix::determinant() const
{
    double a = (*this)(0, 0);
    double b = (*this)(0, 1);
    double c = (*this)(0, 2);
    double d = (*this)(1, 0);
    double e = (*this)(1, 1);
    double f = (*this)(1, 2);
    double g = (*this)(2, 0);
    double h = (*this)(2, 1);
    double i = (*this)(2, 2);

    return a * (e * i - f * h)
         - b * (d * i - f * g)
         + c * (d * h - e * g);
}


Matrix Matrix::transpose() const
{
    Matrix result;

    for (size_t row = 0; row < 3; ++row)
        for (size_t col = 0; col < 3; ++col)
            result(col, row) = (*this)(row, col);

    return result;
}


Matrix Matrix::adjoint() const
{
    Matrix cof;

    cof(0, 0) =  (*this)(1,1) * (*this)(2,2) - (*this)(1,2) * (*this)(2,1);
    cof(0, 1) = -((*this)(1,0) * (*this)(2,2) - (*this)(1,2) * (*this)(2,0));
    cof(0, 2) =  (*this)(1,0) * (*this)(2,1) - (*this)(1,1) * (*this)(2,0);

    cof(1, 0) = -((*this)(0,1) * (*this)(2,2) - (*this)(0,2) * (*this)(2,1));
    cof(1, 1) =  (*this)(0,0) * (*this)(2,2) - (*this)(0,2) * (*this)(2,0);
    cof(1, 2) = -((*this)(0,0) * (*this)(2,1) - (*this)(0,1) * (*this)(2,0));

    cof(2, 0) =  (*this)(0,1) * (*this)(1,2) - (*this)(0,2) * (*this)(1,1);
    cof(2, 1) = -((*this)(0,0) * (*this)(1,2) - (*this)(0,2) * (*this)(1,0));
    cof(2, 2) =  (*this)(0,0) * (*this)(1,1) - (*this)(0,1) * (*this)(1,0);

    return cof.transpose();
}


Matrix Matrix::inverse() const
{
    double det = determinant();
    Matrix adj = adjoint();

    if (det == 0.0)
        throw runtime_error("Matrix is singular; inverse does not exist");

    Matrix inv;

    for (size_t row = 0; row < 3; ++row)
        for (size_t col = 0; col < 3; ++col)
            inv(row, col) = adj(row, col) / det;

    return inv;
}
