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
    Matrix moment_of_inertia = {
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    };

    Vector x = {1, 0, 0};       // Orientation of x axis
    Vector y = {0, 1, 0};       // Orientation of y axis
    Vector z = {0, 0, 1};       // Orientation of z axis
    Vector angular_velocity = {1, 0, 0};
    Vector angular_acceleration = {1, 0, 0};

    // Magnetic moments ...
    double bar_m = 0.1;         // Mag-mom of bar magnet
    Vector bar_dir = {0,0,1};   // Direction of bar magnet
    double hyst_vol = 1.0;      // Volume of hysteress rod
    double hyst_nd = 30;

    // number of hysteresis rods
    int num_x_hyst = 0;
    int num_y_hyst = 0;
    int num_z_hyst = 0;
    Flatley hyst_x;
    Flatley hyst_y;
    Flatley hyst_z;
    double hyst_m_x = 0.0,        // ... of hysteresis rods in x direction
           hyst_m_y = 0.0,        // ... of hysteresis rods in y direction
           hyst_m_z = 0.0;        // ... of hysteresis rods in z direction

public:
    // Constructor
    Satellite();
    Satellite(Matrix in_MOI,
              Vector in_x,
              Vector in_y,
              Vector in_z,
              Vector in_omega,
              Vector in_alpha,
              double in_bar_m,
              double in_hyst_vol,
              double in_hyst_nd,
              int in_num_x_hyst,
              int in_num_y_hyst,
              int in_num_z_hyst,
              double H_c,
              double B_r,
              double B_s,
              double q_0,
              double p);

    // Modifiers
    void set_moment_of_inertia(Matrix MOI);
    void set_orientation(Vector X, Vector Y, Vector Z);
    void set_angular_velocity(Vector omega);
    void set_angular_acceleration(Vector alpha);

    void set_bar_m(double m);
    void set_num_x_hyst(int h);
    void set_num_y_hyst(int h);
    void set_num_z_hyst(int h);

    void set_hyst_curve(double H_c,
                        double B_r,
                        double B_s,
                        double q_0,
                        double p);

    // Update the hystersis values
    void update_hyst_m(Vector H, double timestep);

    // Accessors
    Matrix get_moment_of_inertia() const;
    vector<Vector> get_orientation() const;
    Vector get_angular_velocity() const;
    Vector get_angular_acceleration() const;

    double get_bar_m() const;
    Vector get_hyst_b() const;
    Vector get_hyst_m() const;
    double get_hyst_vol() const;
    double get_hyst_nd() const;
    Vector get_net_m() const;
    int get_num_x_hyst() const;
    int get_num_y_hyst() const;
    int get_num_z_hyst() const;

    // Displayers
    string display_moment_of_inertia() const;
    string display_orientation() const;
    string display_angular_velocity() const;
    string display_angular_acceleration() const;

    // Function to apply torque on the satellite
    void apply_torque(Vector torque, double timestep);
};

#endif
