#ifndef FLATLEY_H
#define FLATLEY_H

#include "Vector.h"
#include <cmath>

class Flatley
{
private:

    static constexpr double pi = 3.141592653;

    // Parameters for defining the hysteresis loop
    double hC = 80;
    double bR = 1.3;
    double bS = 2.1;
    double q0 = 0;
    double p  = 2;
    double k  = std::tan(pi * bR / 2 / bS) / hC;

    // Orientation of the hysteresis material
    double h4 = 0;              // Previous values of Auxiliary Magnetic
    double h3 = 0;              // for finding accurate derivative values
    double h2 = 0;              // only magnitude is stored as only that is
    double h1 = 0;              // required
    double h0 = 0;              // Current values of aux-magnetic field
    Vector n0 = {0,0,1};

    double b0    = 0;           // Current B (magnitude)
    double bPrev = 0;           // Previous B

    // Variables for determining the sign of the curve
    bool slopeSign = 0;
    double tolerance = 0.0f;
    void updateSlopeSign();

public:

    // Constructors
    Flatley();
    Flatley(const double& h0Init,
            const double& h1Init,
            const double& h2Init,
            const double& h3Init,
            const double& h4Init,
            const Vector& n0Init);
    Flatley(const double& h0Init,
            const double& h1Init,
            const double& h2Init,
            const double& h3Init,
            const double& h4Init,
            const Vector& n0Init,
            double hCVal,
            double bRVal,
            double bSVal,
            double q0Val,
            double pVal);

    // Modifier functions
    void setTolerance(double newTolerance);
    void setHC(double hCVal);
    void setBR(double bRVal);
    void setBS(double bSVal);
    void setQ0(double q0Val);
    void setP(double pVal);
    void setParameters(double hCVal,
                       double bRVal,
                       double bSVal,
                       double q0Val,
                       double pVal);

    // Accessor functions
    double getHC() const;
    double getBR() const;
    double getBS() const;
    double getQ0() const;
    double getP() const;
    bool getSlopeSign() const;
    void getParameters(double& hCVal,
                       double& bRVal,
                       double& bSVal,
                       double& q0Val,
                       double& pVal) const;
    Vector getMagField() const;

    // Function to get the magnetic field for next step
    double calcMagField(double timestep, Vector hNew, Vector nNew);
};

#endif // FLATLEY_H

