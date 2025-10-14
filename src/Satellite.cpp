#include "Satellite.h"
#include <iostream>  //debug
#include "Vector.h"
#include <sstream>
#include <cmath>
using namespace std;


// -- -- -- --- //
// CONSTRUCTORS //
// -- -- -- --- //


// Default constructor
Satellite::Satellite()
    : moment_of_inertia({
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
      }),
      x({1,0,0}),
      y({0,1,0}),
      z({0,0,1}),
      angular_velocity({1, 0, 0}),
      angular_acceleration({1, 0, 0})
{}


// Parameterized constructor
Satellite::Satellite(Matrix in_MOI,
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
                     double p)
    : moment_of_inertia(in_MOI),
      x(in_x),
      y(in_y),
      z(in_z),
      angular_velocity(in_omega),
      angular_acceleration(in_alpha),
      bar_m(in_bar_m),
      hyst_vol(in_hyst_vol),
      hyst_nd(in_hyst_nd),
      num_x_hyst(in_num_x_hyst),
      num_y_hyst(in_num_y_hyst),
      num_z_hyst(in_num_z_hyst)
{
    // hyst_m_x, hyst_m_y, hyst_m_z remain 0.0
    Vector H_init_0 = {1,0,0};
    Vector H_init_prev = {1,0,0};
    Vector n_init_0_x = x;
    Vector n_init_prev_x = x;
    Vector n_init_0_y = x;
    Vector n_init_prev_y = x;
    Vector n_init_0_z = x;
    Vector n_init_prev_z = x;

    hyst_x = Flatley(H_init_0, H_init_prev, 
                     n_init_0_x, n_init_prev_x, 
                     H_c, B_r, B_s, q_0, p);
    hyst_y = Flatley(H_init_0, H_init_prev, 
                     n_init_0_y, n_init_prev_y, 
                     H_c, B_r, B_s, q_0, p);
    hyst_z = Flatley(H_init_0, H_init_prev, 
                     n_init_0_z, n_init_prev_z, 
                     H_c, B_r, B_s, q_0, p);
}

// -- -- --- //
// MODIFIERS //
// -- -- --- //


void Satellite::set_moment_of_inertia(Matrix MOI)
{
    moment_of_inertia = MOI;
}


void Satellite::set_orientation(Vector X, Vector Y, Vector Z)
{
    x = X;
    y = Y;
    z = Z;
}


void Satellite::set_angular_velocity(Vector omega)
{
    angular_velocity = omega;
}


void Satellite::set_angular_acceleration(Vector alpha)
{
    angular_acceleration = alpha;
}


void Satellite::set_bar_m(double m)
{
    bar_m = m;
}


void Satellite::update_hyst_m(Vector H, double timestep)
{
    double B_x = hyst_x.calc_mag_field(timestep, H, x);
    // cout << "Returned x mag_field" << endl; //debug
    hyst_m_x = num_x_hyst * hyst_vol * ((B_x / mu_0) - H*x) / (1 - hyst_nd);
    // cout << "Hyst_x updated" << endl; //debug
    double B_y = hyst_y.calc_mag_field(timestep, H, y);
    // cout << "Returned y mag_field" << endl; //debug
    hyst_m_y = num_y_hyst * hyst_vol * ((B_y / mu_0) - H*y) / (1 - hyst_nd);
    // cout << "Hyst_y updated" << endl; //debug
    double B_z = hyst_z.calc_mag_field(timestep, H, z);
    // cout << "Returned z mag_field" << endl; //debug
    hyst_m_z = num_z_hyst * hyst_vol * ((B_z / mu_0) - H*z) / (1 - hyst_nd);
    // cout << "Hyst_z updated" << endl; //debug
}


void Satellite::set_num_x_hyst(int h)
{
    num_x_hyst = h;
}


void Satellite::set_num_y_hyst(int h)
{
    num_y_hyst = h;
}


void Satellite::set_num_z_hyst(int h)
{
    num_z_hyst = h;
}

void Satellite::set_hyst_curve(double H_c,
                               double B_r,
                               double B_s,
                               double q_0,
                               double p)
{
    hyst_x.set_parameters(H_c, B_r, B_s, q_0, p);
    hyst_y.set_parameters(H_c, B_r, B_s, q_0, p);
    hyst_z.set_parameters(H_c, B_r, B_s, q_0, p);
}

// -- -- --- //
// ACCESSORS //
// -- -- --- //
//
Matrix Satellite::get_moment_of_inertia() const
{
    return moment_of_inertia;
}

vector<Vector> Satellite::get_orientation() const
{
    vector<Vector> orientation = {x, y, z};
    return orientation;
}


Vector Satellite::get_angular_velocity() const
{
    return angular_velocity;
}


Vector Satellite::get_angular_acceleration() const
{
    return angular_acceleration;
}


Vector Satellite::get_hyst_m() const
{
    return {hyst_m_x, hyst_m_y, hyst_m_z};
}


double Satellite::get_hyst_vol() const
{
    return hyst_vol;
}


double Satellite::get_hyst_nd() const
{
    return hyst_nd;
}

Vector Satellite::get_hyst_b() const
{
    return  hyst_x.get_mag_field() +
            hyst_y.get_mag_field() +
            hyst_z.get_mag_field();
}


Vector Satellite::get_net_m() const
{
    Vector net_m = {0,0,0};
    net_m += x * hyst_m_x;
    net_m += y * hyst_m_y;
    net_m += z * hyst_m_z;
    return net_m;
}


int Satellite::get_num_x_hyst() const
{
    return num_x_hyst;
}


int Satellite::get_num_y_hyst() const
{
    return num_y_hyst;
}


int Satellite::get_num_z_hyst() const
{
    return num_z_hyst;
}


double Satellite::get_bar_m() const
{
    return bar_m;
}


// -- -- -- - //
// DISPLAYERS //
// -- -- -- - //


string Satellite::display_moment_of_inertia() const
{
    return moment_of_inertia.display();
}


string Satellite::display_orientation() const
{
    ostringstream oss;
    oss << x.display() << endl
        << y.display() << endl
        << z.display() << endl;
    return oss.str();
}


string Satellite::display_angular_velocity() const
{
    return angular_velocity.display();
}


string Satellite::display_angular_acceleration() const
{
    return angular_acceleration.display();
}


// Function to apply torque

void Satellite::apply_torque(Vector torque, double time)
{
    // d_0 = wt + (1/2)at^2
    Vector d_theta = angular_velocity * time + 
        angular_acceleration * (time * time / 2);

    // defining parameters to make rotation matrix
    Vector axis = d_theta.direction();
    double u_x = axis[0], u_y = axis[1], u_z = axis[2];
    double angle = d_theta.magnitude();
    double c = cos(angle);
    double s = sin(angle);

    // Defining rotation matrix
    Matrix R = {
        u_x*u_x*(1-c) + c,      u_x*u_y*(1-c) - u_z*s,  u_x*u_z*(1-c) + u_y*s,
        u_x*u_y*(1-c) + u_z*s,  u_y*u_y*(1-c) + c,      u_y*u_z*(1-c) - u_x*s,
        u_x*u_z*(1-c) - u_y*s,  u_y*u_z*(1-c) + u_x*s,  u_z*u_z*(1-c) + c
    };

    // Updating values (position to velocity to angular velocity)
    x = (R * x).direction();    // changing orientation
    y = (R * y).direction();
    z = (R * z).direction();
    angular_velocity += angular_acceleration * time;
    angular_acceleration = moment_of_inertia.inverse() * torque;
}
