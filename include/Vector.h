// Vector.h
#ifndef VECTOR_H
#define VECTOR_H

#include <vector>
#include <cstddef>
#include <string>
#include <initializer_list>
using namespace std;

class Vector
{
private:
    vector<double> data;

    void checkBounds(size_t index) const;

public:
    // Constructors
    Vector();
    Vector(initializer_list<double> values);

    // return an element of the vector
    double& operator[](size_t index);
    const double& operator[](size_t index) const;

    // return the size of the vector
    size_t size() const;

    // return the value of a particular index of the vector
    double at(size_t index) const;

    // Operators
    Vector operator*(double scalar) const;        // Scale
    Vector operator/(double scalar) const;        // Scale
    double operator*(const Vector& other) const;  // Dot Product
    Vector operator^(const Vector& other) const;  // Cross Product
    Vector operator+(const Vector& other) const;  // Vector Addition
    Vector operator-(const Vector& other) const;  // Vector Addition
    Vector& operator+=(const Vector& other);      // Vector Addition

    // Functions to get characteristic values of a vector
    double magnitude() const;       // returns magnitude
    Vector direction() const;       // returns direction (unit vector)

    // Operator to return the value of the string
    string display() const;
};

#endif // VECTOR_H
