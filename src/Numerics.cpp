#include "Numerics.h"
#include "DateTime.h"
#include <algorithm>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include <iostream>


/* ================= SamplePoint1D ================= */

SamplePoint1D::SamplePoint1D(const DateTime& time,
                             double val)
    : t(time), y(val)
{
}

bool SamplePoint1D::operator<(const SamplePoint1D& other) const
{
    return t < other.t;
}


/* ================= SampleData1D ================= */

SampleData1D::SampleData1D()
    : position(0), sorted_(true)
{
}

SampleData1D::SampleData1D(const string& filename,
                           const string& timeColumn,
                           const string& valueColumn)
    : position(0), sorted_(true)
{
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Cannot open CSV file");

    string line;
    vector<string> header;

    std::getline(file, line);
    std::stringstream hs(line);

    string token;
    while (std::getline(hs, token, ','))
        header.push_back(token);

    size_t tIdx = findColumn(header, timeColumn);
    size_t vIdx = findColumn(header, valueColumn);

    while (std::getline(file, line))
    {
        std::stringstream ss(line);
        vector<string> values;

        while (std::getline(ss, token, ','))
            values.push_back(token);

        DateTime t(values[tIdx]);
        double y = std::stod(values[vIdx]);

        data.emplace_back(t, y);
    }

    sorted_ = checkSort();
}

size_t SampleData1D::findColumn(const vector<string>& header,
                                const string& name) const
{
    for (size_t i = 0; i < header.size(); ++i)
        if (header[i] == name)
            return i;

    throw std::runtime_error("Column not found: " + name);
}

void SampleData1D::addSample(const DateTime& t,
                             double y)
{
    data.emplace_back(t, y);
    sorted_ = false;
}

void SampleData1D::sort()
{
    std::sort(data.begin(), data.end());
    sorted_ = true;
    position = 0;
}

bool SampleData1D::checkSort() const
{
    for (size_t i = 1; i < data.size(); ++i)
    {
        if (data[i] < data[i - 1])
            return false;
    }
    return true;
}

bool SampleData1D::isSorted() const
{
    return sorted_;
}

void SampleData1D::requireSorted() const
{
    if (!sorted_)
        throw std::runtime_error("SampleData1D not sorted");
}

void SampleData1D::setPosition(const DateTime time) const{

    if (data.empty()){
        throw std::runtime_error("SampleDataVector Empty");
    }

    /* Move forward if t is ahead */
    while (position + 1 < data.size() &&
           !(time < data[position + 1].t)){
        ++position;
    }

    /* Move backward if t is behind */
    while (position > 0 &&
           time < data[position].t){
        --position;
    }

    // making sure the skeleton does not go out of the range of values
    if (data.size() <= 4){
        throw std::runtime_error("Less than four data values provided");
    } else if (position < 1){
        position = 1;
    } else if (position + 3 >= data.size()){
        position = data.size() - 3;
    }
}

double SampleData1D::linearInterpolate(const DateTime& t) const
{
    requireSorted();

    if (data.empty()) {
    throw std::runtime_error(
            "Interpolation requested on empty SampleDataVector"
        );
    }

    /* Move forward if t is ahead */
    while (position + 1 < data.size() &&
           !(t < data[position + 1].t)){
        ++position;
    }

    /* Move backward if t is behind */
    while (position > 0 &&
           t < data[position].t){
        --position;
    }

    // Checking if position has exceeded data size
    if (position >= data.size()) {
        position = data.size() - 1;
    }


    const auto& p0 = data[position];
    const auto& p1 = data[position + 1];

    double dt = t - p0.t;
    double dtTot = p1.t - p0.t;

    return p0.y +
           (p1.y - p0.y) * (dt / dtTot);
}

double SampleData1D::lagrangeInterpolate(const DateTime& t) const
{
    requireSorted();
    setPosition(t);

    size_t i0 = (position > 0) ? position - 1 : position;
    size_t i1 = position;
    size_t i2 = position + 1;
    size_t i3 = (position + 2 < data.size())
                ? position + 2
                : position + 1;

    const SamplePoint1D* p[4] =
        { &data[i0], &data[i1], &data[i2], &data[i3] };

    double result = 0.0;

    for (int i = 0; i < 4; ++i)
    {
        double term = p[i]->y;

        for (int j = 0; j < 4; ++j)
        {
            if (i != j)
                term *= (t - p[j]->t) /
                        (p[i]->t - p[j]->t);
        }
        result += term;
    }

    return result;
}

size_t SampleData1D::size() const
{
    return data.size();
}

SampleData1D SampleData1D::operator*(double scalar) const
{
    SampleData1D result;

    result.data.reserve(data.size());

    for (const auto& p : data)
    {
        result.data.emplace_back(p.t, p.y * scalar);
    }

    result.sorted_ = sorted_;
    result.position = position;

    return result;
}


/* ================= SamplePointVector ================= */

SamplePointVector::SamplePointVector(const DateTime& time,
                                     const Vector& val)
    : t(time), y(val)
{
}

bool SamplePointVector::operator<(const SamplePointVector& other) const
{
    return t < other.t;
}


/* ================= SampleDataVector ================= */

SampleDataVector::SampleDataVector()
    : position(0), sorted_(true)
{
}

SampleDataVector::SampleDataVector(const string& filename,
                                   const string& timeColumn,
                                   const string& colX,
                                   const string& colY,
                                   const string& colZ)
    : position(0), sorted_(true)
{
    std::ifstream file(filename);
    if (!file)
        throw std::runtime_error("Cannot open CSV file");

    string line;
    vector<string> header;

    std::getline(file, line);
    std::stringstream hs(line);

    string token;
    while (std::getline(hs, token, ','))
        header.push_back(token);

    size_t tIdx = findColumn(header, timeColumn);
    size_t xIdx = findColumn(header, colX);
    size_t yIdx = findColumn(header, colY);
    size_t zIdx = findColumn(header, colZ);


    while (std::getline(file, line))
    {
        if (line.empty())
            continue;

        std::stringstream ss(line);
        vector<string> values;
        string token;

        while (std::getline(ss, token, ','))
            values.push_back(token);

        /* Check column bounds */
        if (tIdx >= values.size() ||
            xIdx >= values.size() ||
            yIdx >= values.size() ||
            zIdx >= values.size()
            ){
            std::cerr << "Skipping malformed line (column missing)\n";
            continue;
        }

        /* Construct DateTime safely */
        DateTime t;
        try{
            t = DateTime(values[tIdx]);
        }
        catch (...){
            std::cerr << "Invalid DateTime: "
                      << values[tIdx] << '\n';
            continue;
        }

        /* Convert vector components safely */
        double vx, vy, vz;

        try{
            vx = std::stod(values[xIdx]);
            vy = std::stod(values[yIdx]);
            vz = std::stod(values[zIdx]);
        }
        catch (...){
            std::cerr << "Invalid numeric data at time "
                      << t.display() << '\n';
            continue;
        }

        /* Diagnostic print BEFORE storing 
        For Debug only
        std::cout << "Time   : " << t.display() << '\n'
                  << "Vector : "
                  << vx << ", "
                  << vy << ", "
                  << vz << "\n\n";
        */

        Vector v = { vx, vy, vz };
        data.emplace_back(t, v);
    }
    sorted_ = checkSort();
}

size_t SampleDataVector::findColumn(const vector<string>& header,
                                    const string& name) const
{
    for (size_t i = 0; i < header.size(); ++i)
        if (header[i] == name)
            return i;

    throw std::runtime_error("Column not found: " + name);
}

void SampleDataVector::addSample(const DateTime& t,
                                 const Vector& y)
{
    data.emplace_back(t, y);
    sorted_ = false;
}

void SampleDataVector::sort()
{
    std::sort(data.begin(), data.end());
    sorted_ = true;
    position = 0;
}

bool SampleDataVector::checkSort() const
{
    for (size_t i = 1; i < data.size(); ++i)
    {
        if (data[i] < data[i - 1])
            return false;
    }
    return true;
}

bool SampleDataVector::isSorted() const
{
    return sorted_;
}

void SampleDataVector::requireSorted() const
{
    if (!sorted_)
        throw std::runtime_error("SampleDataVector not sorted");
}

void SampleDataVector::setPosition(const DateTime time) const{

    if (data.empty()){
        throw std::runtime_error("SampleDataVector Empty");
    }

    /* Move forward if t is ahead */
    while (position + 1 < data.size() &&
           !(time < data[position + 1].t)){
        ++position;
    }

    /* Move backward if t is behind */
    while (position > 0 &&
           time < data[position].t){
        --position;
    }

    // making sure the skeleton does not go out of the range of values
    if (data.size() <= 4){
        throw std::runtime_error("Less than four data values provided");
    } else if (position < 1){
        position = 1;
    } else if (position + 3 >= data.size()){
        position = data.size() - 3;
    }
}


Vector SampleDataVector::linearInterpolate(const DateTime& t) const{
    requireSorted();

    setPosition(t);

    const auto& p0 = data[position];
    const auto& p1 = data[position + 1];

    double dt = t - p0.t;
    double dtTot = p1.t - p0.t;

    return p0.y +
           (p1.y - p0.y) * (dt / dtTot);
}

Vector SampleDataVector::lagrangeInterpolate(const DateTime& t) const{
    requireSorted();

    /* Move forward if t is ahead */
    while (position + 1 < data.size() &&
           !(t < data[position + 1].t)){
        ++position;
    }

    /* Move backward if t is behind */
    while (position > 0 &&
           t < data[position].t){
        --position;
    }

    // Checking if position has exceeded data size
    if (position >= data.size()) {
        position = data.size() - 1;
    }

    size_t i0 = (position > 0) ? position - 1 : position;
    size_t i1 = position;
    size_t i2 = position + 1;
    size_t i3 = (position + 2 < data.size())
                ? position + 2
                : position + 1;

    const SamplePointVector* p[4] =
        { &data[i0], &data[i1], &data[i2], &data[i3] };

    Vector result = {0, 0, 0};

    for (int i = 0; i < 4; ++i){
        double coeff = 1.0;

        for (int j = 0; j < 4; ++j){
            if (i != j)
                coeff *= (t - p[j]->t) / (p[i]->t - p[j]->t);
        }
        result = result + p[i]->y * coeff;
    }

    return result;
}

size_t SampleDataVector::size() const
{
    return data.size();
}

SampleDataVector SampleDataVector::operator*(double scalar) const
{
    SampleDataVector result;

    result.data.reserve(data.size());

    for (const auto& p : data)
    {
        result.data.emplace_back(p.t, p.y * scalar);
    }

    result.sorted_ = sorted_;
    result.position = position;

    return result;
}
