#include "Flatley.h"
#include <cmath>
using namespace std;


void Flatley::updateSlopeSign(){
    // updates the value of slope if slope exceeds some tolerance
    // double BdH = (25*h0 - 48*h1 - 36*h3 + 3*h4)/(12);
    double BdH = h0 - h1;
    if (BdH < ( - tolerance))
        // Negative slope gives 0
        slopeSign = 0;
    else if (BdH > tolerance)
        // Positive slope gives 1
        slopeSign = 1;
}


// NOTE::
    // -- -- --- //
    // MODIFIERS //
    // -- -- --- //


void Flatley::setTolerance(double newTolerance){
    tolerance = newTolerance;
}


void Flatley::setHC(double hCVal){
    hC = hCVal;
}


void Flatley::setBR(double bRVal){
    bR = bRVal;
    k = tan(pi * bR / (2 * bS));
}


void Flatley::setBS(double bSVal){
    bS = bSVal;
    k = tan(pi * bR / (2 * bS));
}


void Flatley::setQ0(double q0Val){
    q0 = q0Val;
}


void Flatley::setP(double pVal){
    p = pVal;
}

void Flatley::setParameters(double hCVal, double bRVal,
                             double bSVal, double q0Val, double pVal){
    hC = hCVal;
    bR = bRVal;
    bS = bSVal;
    q0 = q0Val;
    p = pVal;

    k = tan(pi * bR / (2 * bS));
}


// NOTE::
    // -- -- --- //
    // ACCESSORS //
    // -- -- --- //


double Flatley::getHC() const{
    return hC;
}


double Flatley::getBR() const{
    return bR;
}


double Flatley::getBS() const{
    return bS;
}


double Flatley::getQ0() const{
    return q0;
}


double Flatley::getP() const{
    return p;
}

bool Flatley::getSlopeSign() const{
    return slopeSign;
}

void Flatley::getParameters(double& hCVal,
                             double& bRVal,
                             double& bSVal,
                             double& q0Val,
                             double& pVal) const{
    hCVal = hC;
    bRVal = bR;
    bSVal = bS;
    q0Val = q0;
    pVal = p;
}

Vector Flatley::getMagField() const{
    return n0 * b0;
}

// Function to get new magnetic field
double Flatley::calcMagField(double dt, Vector hNew, Vector nNew){
    h4 = h3;
    h3 = h2;
    h2 = h1;
    h1 = h0;
    h0 = hNew * nNew;
    n0 = nNew;

    updateSlopeSign();

    int numSubsteps = 10;

    double theta = (pi * b0) / (2 * bS);
    double k0 = tan(theta) / (k * hC);
    double beta = 0;
    double alpha = (q0 + (1 - q0) * beta) * (2 * k * bS / pi);
    double cos2 = pow(cos(theta), 2);
    double BdH = (h0 - h1)/numSubsteps;
    double dB = (alpha * cos2 * BdH);
    double bNew = 0;

    for (int i = 0; i < numSubsteps; i++){
        /*
        // double BdH = (25*h0 - 48*h1 - 36*h3 + 3*h4)/(12);
        theta = (pi * b0) / (2 * bS);
        k0 = tan(theta) / (k * hC);
        if (slopeSign == 0)
            beta = pow((h0 - k0 - hC) / (2 * hC), p);
        else
            beta = pow((h0 - k0 + hC) / (2 * hC), p);
        alpha = (q0 + (1 - q0) * beta) * (2 * k * bS / pi);
        cos2 = pow(cos(theta), 2);
        BdH = (h0 - h1)/numSubsteps;

        dB = (alpha * cos2 * BdH / dt);

        bNew = b0 + dB;

        // Limits of magnetic field from inverse tangent model
        double bMax = (2*bS/pi)*atan(k*(h0 + hC));
        double bMin = (2*bS/pi)*atan(k*(h0 - hC));
        if (bNew > bMax){
            bNew = bMax;
        } else if(bNew < bMin) {
            bNew = bMin;
        }
        */

        // Inverse tangent model
        if (slopeSign == 0){
            bNew = (2*bS/pi)*atan(k*(h0 + hC));
        } else {
            bNew = (2*bS/pi)*atan(k*(h0 - hC));
        };

        bPrev = b0;
        b0 = bNew;
    };

    return b0;
}


// NOTE::
    // -- -- -- --- //
    // CONSTRUCTORS //
    // -- -- -- --- //


// Default constructor
Flatley::Flatley()
{}


// Constructor with timestep and initial vectors
Flatley::Flatley(
    const double& h0Init,
    const double& h1Init,
    const double& h2Init,
    const double& h3Init,
    const double& h4Init,
    const Vector& n0Init
)
    : h0(h0Init),
      h1(h1Init),
      h2(h2Init),
      h3(h3Init),
      h4(h4Init),
      n0(n0Init)
{}


// Constructor with most specifications
Flatley::Flatley(const double& h0Init,
                 const double& h1Init,
                 const double& h2Init,
                 const double& h3Init,
                 const double& h4Init,
                 const Vector& n0Init,
                 double hCVal,
                 double bRVal,
                 double bSVal,
                 double q0Val,
                 double pVal)
    : hC(hCVal),
      bR(bRVal),
      bS(bSVal),
      q0(q0Val),
      p(pVal),
      k(tan(pi * bRVal / (2 * bSVal))/hCVal),
      h0(h0Init),
      h1(h1Init),
      h2(h2Init),
      h3(h3Init),
      h4(h4Init),
      n0(n0Init),
      slopeSign(false)
{}
