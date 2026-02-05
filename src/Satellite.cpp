#include "Satellite.h"
#include <iostream>  //debug
#include "Matrix.h"
#include "Vector.h"
#include <sstream>
#include <cmath>
using namespace std;


// -- -- -- --- //
// CONSTRUCTORS //
// -- -- -- --- //


// Default constructor
Satellite::Satellite()
    : momentOfInertia({
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
      }),
      x({1,0,0}),
      y({0,1,0}),
      z({0,0,1}),
      angularVelocity({1, 0, 0}),
      angularAcceleration({1, 0, 0})
{}


// Parameterized constructor
Satellite::Satellite(Matrix inMOI,
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
                     double p)
    : momentOfInertia(inMOI),
      x(inX),
      y(inY),
      z(inZ),
      angularVelocity(inOmega),
      angularAcceleration(inAlpha),
      barM(inBarM),
      hystVol(inHystVol),
      hystNd(inHystNd),
      numXHyst(inNumXHyst),
      numYHyst(inNumYHyst),
      numZHyst(inNumZHyst)
{
    // hystMX, hystMY, hystMZ remain 0.0
    double h0Init = 0;
    double h1Init = 0;
    double h2Init = 0;
    double h3Init = 0;
    double h4Init = 0;
    Vector n0InitX = x;
    Vector n0InitY = x;
    Vector n0InitZ = x;

    hystX = Flatley(h0Init, h1Init, h2Init, h3Init, h4Init, 
                    n0InitX,
                    hC, bR, bS, q0, p);
    hystY = Flatley(h0Init, h1Init, h2Init, h3Init, h4Init, 
                    n0InitY,
                    hC, bR, bS, q0, p);
    hystZ = Flatley(h0Init, h1Init, h2Init, h3Init, h4Init, 
                    n0InitZ,
                    hC, bR, bS, q0, p);
    Matrix R = {
        x[0], y[0], z[0],
        x[1], y[1], z[1],
        x[2], y[2], z[2]
    };
    // momentOfInertia = R * momentOfInertia * R.transpose();
}


// -- -- --- //
// MODIFIERS //
// -- -- --- //


void Satellite::setMomentOfInertia(Matrix MOI){
    momentOfInertia = MOI;
}


void Satellite::setOrientation(Vector X, Vector Y, Vector Z){
    x = X;
    y = Y;
    z = Z;
}


void Satellite::setAngularVelocity(Vector omega){
    angularVelocity = omega;
}


void Satellite::setAngularAcceleration(Vector alpha){
    angularAcceleration = alpha;
}


void Satellite::setBarM(double m){
    barM = m;
}


void Satellite::updateHystM(Vector H, double timestep){
    double BX = hystX.calcMagField(timestep, H, x);
    // cout << "Returned x mag_field" << endl; //debug
    hystMX = numXHyst * hystVol * ((BX / mu_0) - H*x) / (1 - hystNd);
    // cout << "Hyst_x updated" << endl; //debug
    double BY = hystY.calcMagField(timestep, H, y);
    // cout << "Returned y mag_field" << endl; //debug
    hystMY = numYHyst * hystVol * ((BY / mu_0) - H*y) / (1 - hystNd);
    // cout << "Hyst_y updated" << endl; //debug
    double BZ = hystZ.calcMagField(timestep, H, z);
    // cout << "Returned z mag_field" << endl; //debug
    hystMZ = numZHyst * hystVol * ((BZ / mu_0) - H*z) / (1 - hystNd);
    // cout << "Hyst_z updated" << endl; //debug
}


void Satellite::setNumXHyst(int h){
    numXHyst = h;
}


void Satellite::setNumYHyst(int h){
    numYHyst = h;
}


void Satellite::setNumZHyst(int h){
    numZHyst = h;
}

void Satellite::setHystCurve(double hC,
                              double bR,
                              double bS,
                              double q0,
                              double p){
    hystX.setParameters(hC, bR, bS, q0, p);
    hystY.setParameters(hC, bR, bS, q0, p);
    hystZ.setParameters(hC, bR, bS, q0, p);
}

// -- -- --- //
// ACCESSORS //
// -- -- --- //
//
Matrix Satellite::getMomentOfInertia() const{
    return momentOfInertia;
}

vector<Vector> Satellite::getOrientation() const{
    vector<Vector> orientation = {x, y, z};
    return orientation;
}


Vector Satellite::getAngularVelocity() const{
    return angularVelocity;
}


Vector Satellite::getAngularAcceleration() const{
    return angularAcceleration;
}


Vector Satellite::getHystM() const{
    return {hystMX, hystMY, hystMZ};
}


double Satellite::getHystVol() const{
    return hystVol;
}


double Satellite::getHystNd() const{
    return hystNd;
}

Vector Satellite::getHystB() const{
    return  hystX.getMagField() +
            hystY.getMagField() +
            hystZ.getMagField();
}


Vector Satellite::getNetM() const{
    Vector netM = {0,0,0};
    netM += x * hystMX;
    netM += y * hystMY;
    netM += z * hystMZ;
    return netM;
}


int Satellite::getNumXHyst() const{
    return numXHyst;
}


int Satellite::getNumYHyst() const{
    return numYHyst;
}


int Satellite::getNumZHyst() const{
    return numZHyst;
}


double Satellite::getBarM() const{
    return barM;
}


// -- -- -- - //
// DISPLAYERS //
// -- -- -- - //


string Satellite::displayMomentOfInertia() const{
    return momentOfInertia.display();
}


string Satellite::displayOrientation() const{
    ostringstream oss;
    oss << x.display() << endl
        << y.display() << endl
        << z.display() << endl;
    return oss.str();
}


string Satellite::displayAngularVelocity() const{
    return angularVelocity.display();
}


string Satellite::displayAngularAcceleration() const{
    return angularAcceleration.display();
}


// Function to apply torque

void Satellite::applyTorque(Vector torque, double time){
    // d_0 = wt + (1/2)at^2
    // Vector dTheta = angularVelocity * time + 
        // angularAcceleration * (time * time / 2);
    Vector dTheta = angularVelocity * time;

    // defining parameters to make rotation matrix
    Vector axis = dTheta.direction();
    double uX = axis[0], uY = axis[1], uZ = axis[2];
    double angle = dTheta.magnitude();
    double c = cos(angle);
    double s = sin(angle);

    // Defining rotation matrix
    Matrix R = {
        uX*uX*(1-c) + c,      uX*uY*(1-c) - uZ*s,  uX*uZ*(1-c) + uY*s,
        uX*uY*(1-c) + uZ*s,  uY*uY*(1-c) + c,      uY*uZ*(1-c) - uX*s,
        uX*uZ*(1-c) - uY*s,  uY*uZ*(1-c) + uX*s,  uZ*uZ*(1-c) + c
    };

    // Updating values (position to velocity to angular velocity)
    // changing orientation
    x = (R * x).direction();
    y = (R * y).direction();
    z = (R * z).direction();

    angularVelocity += angularAcceleration * time;
    // momentOfInertia = R * momentOfInertia * R.transpose();
    angularAcceleration = momentOfInertia.inverse() * torque;
}
