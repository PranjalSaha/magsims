#include "Vector.h"
#include "Debug.h"
#include <sstream>
#include <stdexcept>
#include <iomanip>
#include <cmath>
using namespace std;

void Vector::checkBounds(size_t index) const
{
    if (index >= 3)
        throw out_of_range(
            "Vector index out of range\n" +
            CallStackTracker::trace()
        );
}

Vector::Vector()
    : data(3, 0.0)
{}

Vector::Vector(initializer_list<double> values)
    : data(values)
{
    if (values.size() != 3)
        throw invalid_argument(
            "Vector must have exactly 3 elements\n" +
            CallStackTracker::trace()
        );
}

// -- -- -- -- --- //
// ACCESS ELEMENTS //
// -- -- -- -- --- //

double& Vector::operator[](size_t index)
{
    checkBounds(index);
    return data[index];
}

const double& Vector::operator[](size_t index) const
{
    checkBounds(index);
    return data[index];
}

double Vector::at(size_t index) const
{
    checkBounds(index);
    return data[index];
}

size_t Vector::size() const
{
    return 3;
}

// -- -- --- //
// OPERATORS //
// -- -- --- //

Vector Vector::operator*(double scalar) const
{
    return Vector{
        data[0] * scalar,
        data[1] * scalar,
        data[2] * scalar
    };
}

Vector Vector::operator/(double scalar) const
{
    return Vector{
        data[0] / scalar,
        data[1] / scalar,
        data[2] / scalar
    };
}

double Vector::operator*(const Vector& other) const
{
    return data[0] * other[0]
         + data[1] * other[1]
         + data[2] * other[2];
}


Vector Vector::operator^(const Vector& other) const
{
    return Vector{
        data[1] * other[2] - data[2] * other[1],
        data[2] * other[0] - data[0] * other[2],
        data[0] * other[1] - data[1] * other[0]
    };
}

Vector Vector::operator+(const Vector& other) const
{
    return Vector{
        (*this)[0] + other[0],
        (*this)[1] + other[1],
        (*this)[2] + other[2]
    };
}

Vector Vector::operator-(const Vector& other) const
{
    return Vector{
        (*this)[0] - other[0],
        (*this)[1] - other[1],
        (*this)[2] - other[2]
    };
}

Vector& Vector::operator+=(const Vector& other)
{
    for (size_t i = 0; i < 3; ++i)
        data[i] += other[i];
    return *this;
}

double Vector::magnitude() const
{
    return sqrt(
        data[0] * data[0] +
        data[1] * data[1] +
        data[2] * data[2]
    );
}

Vector Vector::direction() const
{
    double mag = magnitude();
    if (mag == 0.0)
        throw domain_error(
            std::string(
                "Normalising zero vector\n" +
                CallStackTracker::trace()
            )
        );
    return (*this) * (1.0 / mag);
}


string Vector::display() const
{
    ostringstream oss;
    oss << fixed << setprecision(2);
    oss << "[";
    for (size_t index = 0; index < 3; ++index)
        oss << setw(9) << (*this)[index];
    oss << "]";
    return oss.str();
}
