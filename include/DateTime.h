#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <chrono>
using namespace std;

struct DateTime
{
    // Constructors
    DateTime();
    DateTime(const std::string &time_str);

    // Accessor
    std::string toString() const;

    // Comparison operators
    bool operator<(const DateTime &other) const;
    bool operator>(const DateTime &other) const;
    bool operator==(const DateTime &other) const;

    // Difference in seconds
    double operator-(const DateTime &other) const;
    // Add time to operator
    DateTime operator+(const double seconds) const;

    // returns time as a string
    string display() const;

private:
    std::chrono::system_clock::time_point tp;

    static std::chrono::system_clock::time_point 
        parseDateTime(const std::string &time_str);
};

#endif
