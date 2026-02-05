#ifndef NUMERICS_H
#define NUMERICS_H

#include <vector>
#include <string>
#include "Vector.h"
#include "DateTime.h"

using std::vector;
using std::string;


/* ================= SampleData1D ================= */

struct SamplePoint1D
{
    DateTime t;
    double y;

    SamplePoint1D(const DateTime& time,
                  double val);

    bool operator<(const SamplePoint1D& other) const;
};


class SampleData1D
{
private:
    vector<SamplePoint1D> data;
    mutable size_t position;
    bool sorted_;

    void requireSorted() const;
    size_t findColumn(const vector<string>& header,
                      const string& name) const;
    void setPosition(const DateTime t) const;            // changes position

public:
    SampleData1D();

    SampleData1D(const string& filename,
                 const string& timeColumn,
                 const string& valueColumn);

    void addSample(const DateTime& t,
                   double y);

    void sort();
    bool checkSort() const;
    bool isSorted() const;

    double linearInterpolate(const DateTime& t) const;
    double lagrangeInterpolate(const DateTime& t) const;

    size_t size() const;

    // Operators
    SampleData1D operator*(double scalar) const;
};


/* ================= SampleDataVector ================= */

struct SamplePointVector
{
    DateTime t;
    Vector y;

    SamplePointVector(const DateTime& time,
                      const Vector& val);

    // Operators
    bool operator<(const SamplePointVector& other) const;
};


class SampleDataVector
{
private:
    vector<SamplePointVector> data;
    mutable size_t position = 0;
    bool sorted_ = false;

    void requireSorted() const;
    size_t findColumn(const vector<string>& header,
                      const string& name) const;
    void setPosition(const DateTime t) const;            // changes position

public:
    SampleDataVector();

    SampleDataVector(const string& filename,
                     const string& timeColumn,
                     const string& colX,
                     const string& colY,
                     const string& colZ);

    void addSample(const DateTime& t,
                   const Vector& y);

    void sort();
    bool checkSort() const;
    bool isSorted() const;

    Vector linearInterpolate(const DateTime& t) const;
    Vector lagrangeInterpolate(const DateTime& t) const;

    size_t size() const;

    // Operators
    SampleDataVector operator*(double scalar) const;
};

#endif /* NUMERICS_H */
