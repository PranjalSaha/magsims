#ifndef SATELLITE_H
#define SATELLITE_H

#include "Matrix.h"
#include "Vector.h"
#include "Flatley.h"
using namespace std;

static const double mu_0 = 1.257E-6;

class Satellite
{

private:
    Matrix momentOfInertia = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };

    Vector x = {1, 0, 0};       // Orientation of x axis
    Vector y = {0, 1, 0};       // Orientation of y axis
    Vector z = {0, 0, 1};       // Orientation of z axis
    Vector angularVelocity = {1, 0, 0};
    Vector angularAcceleration = {1, 0, 0};

    // Magnetic moments ...
    double barM = 0.1;          // Mag-mom of bar magnet
    Vector barDir = {0,0,1};    // Direction of bar magnet
    double hystVol = 1.0;       // Volume of hysteress rod
    double hystNd = 30;

    // number of hysteresis rods
    int numXHyst = 0;
    int numYHyst = 0;
    int numZHyst = 0;
    Flatley hystX;
    Flatley hystY;
    Flatley hystZ;
    double hystMX = 0.0,        // ... of hysteresis rods in x direction
           hystMY = 0.0,        // ... of hysteresis rods in y direction
           hystMZ = 0.0;        // ... of hysteresis rods in z direction

public:
    // Constructor
    Satellite();
    Satellite(Matrix inMOI,
              Vector inX,
              Vector inY,
              Vector inZ,
              Vector inOmega,
              Vector inAlpha,
              double inBarM,
              double inHystVol,
              double inHystNd,
              int inNumXHyst,
              int inNumYHyst,
              int inNumZHyst,
              double hC,
              double bR,
              double bS,
              double q0,
              double p);

    // Modifiers
    void setMomentOfInertia(Matrix MOI);
    void setOrientation(Vector X, Vector Y, Vector Z);
    void setAngularVelocity(Vector omega);
    void setAngularAcceleration(Vector alpha);

    void setBarM(double m);
    void setNumXHyst(int h);
    void setNumYHyst(int h);
    void setNumZHyst(int h);

    void setHystCurve(double hC,
                      double bR,
                      double bS,
                      double q0,
                      double p);

    // Update the hystersis values
    void updateHystM(Vector H, double timestep);

    // Accessors
    Matrix getMomentOfInertia() const;
    vector<Vector> getOrientation() const;
    Vector getAngularVelocity() const;
    Vector getAngularAcceleration() const;

    double getBarM() const;
    Vector getHystB() const;
    Vector getHystM() const;
    double getHystVol() const;
    double getHystNd() const;
    Vector getNetM() const;
    int getNumXHyst() const;
    int getNumYHyst() const;
    int getNumZHyst() const;

    // Displayers
    string displayMomentOfInertia() const;
    string displayOrientation() const;
    string displayAngularVelocity() const;
    string displayAngularAcceleration() const;

    // Function to apply torque on the satellite
    void applyTorque(Vector torque, double timestep);
};

#endif
