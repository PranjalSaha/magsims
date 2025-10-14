#ifndef FLATLEY_H
#define FLATLEY_H

#include "Vector.h"
#include <cmath>

class Flatley
{
private:

    static constexpr double pi = 3.141592653;

    // Parameters for defining the hysteresis loop
    double H_c = 80;
    double B_r = 1.3;
    double B_s = 2.1;
    double q_0 = 0;
    double p   = 2;
    double k   = std::tan(pi * B_r / 2 / B_s) / H_c;

    // Orientation of the hysteresis material
    Vector H_0 = {1,0,0};       // Current H
    Vector H_prev = {1,1,0};    // Previous H
    Vector n_0 = {0,0,1};       // Current orientation
    Vector n_prev = {0,0,1};    // Previous orientation

    double B_0    = 0;          // Current B (magnitude)
    double B_prev = 0;          // Previous B

    // Variables for determining the sign of the curve
    bool slope_sign = 0;
    double tolerance = 0.0f;
    void update_slope_sign();

public:

    // Constructors
    Flatley();
    Flatley(Vector, Vector, Vector, Vector);
    Flatley(const Vector& H_init_0,
            const Vector& H_init_prev,
            const Vector& n_init_0,
            const Vector& n_init_prev,
            double H_c_val,
            double B_r_val,
            double B_s_val,
            double q_0_val,
            double p_val);

    // Modifier functions
    void set_tolerance(double new_tolerance);
    void set_H_c(double H_c_val);
    void set_B_r(double B_r_val);
    void set_B_s(double B_s_val);
    void set_q_0(double q_0_val);
    void set_p(double p_val);
    void set_parameters(double H_c_val,
                        double B_r_val,
                        double B_s_val,
                        double q_0_val,
                        double p_val);

    // Accessor functions
    double get_H_c() const;
    double get_B_r() const;
    double get_B_s() const;
    double get_q_0() const;
    double get_p() const;
    bool get_slope_sign() const;
    void get_parameters(double& H_c_val,
                        double& B_r_val,
                        double& B_s_val,
                        double& q_0_val,
                        double& p_val) const;
    Vector get_mag_field() const;

    // Function to get the magnetic field for next step
    double calc_mag_field(double timestep, Vector H_new, Vector n_new);
};

#endif // FLATLEY_H

