#ifndef DATETIME_H
#define DATETIME_H

#include <string>
#include <chrono>
using namespace std;

struct DateTime
{
    // Constructor
    DateTime(const std::string &time_str);

    // Accessor
    std::string to_string() const;

    // Comparison operators
    bool operator<(const DateTime &other) const;
    bool operator>(const DateTime &other) const;
    bool operator==(const DateTime &other) const;

    // Difference in seconds
    double operator-(const DateTime &other) const;

    // returns time as a string
    string display() const;

private:
    std::chrono::system_clock::time_point tp;

    static std::chrono::system_clock::time_point 
        parse_datetime(const std::string &time_str);
};

#endif
