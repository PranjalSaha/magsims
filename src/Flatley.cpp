#include "Flatley.h"
using namespace std;


void Flatley::update_slope_sign(){
    // updates the value of slope if slope exceeds some tolerance
    double Bd_H = H_0 * n_0.direction() - (H_prev * n_prev.direction());
    if (Bd_H < ( - tolerance))
        slope_sign = 0;
    else if (Bd_H > tolerance)
        slope_sign = 1;
}


// NOTE::
    // -- -- --- //
    // MODIFIERS //
    // -- -- --- //


void Flatley::set_tolerance(double new_tolerance){
    tolerance = new_tolerance;
}


void Flatley::set_H_c(double H_c_val){
    H_c = H_c_val;
}


void Flatley::set_B_r(double B_r_val){
    B_r = B_r_val;
    k = tan(pi * B_r / (2 * B_s));
}


void Flatley::set_B_s(double B_s_val){
    B_s = B_s_val;
    k = tan(pi * B_r / (2 * B_s));
}


void Flatley::set_q_0(double q_0_val){
    q_0 = q_0_val;
}


void Flatley::set_p(double p_val){
    p = p_val;
}

void Flatley::set_parameters(double H_c_val, double B_r_val,
                              double B_s_val, double q_0_val, double p_val){
    H_c = H_c_val;
    B_r = B_r_val;
    B_s = B_s_val;
    q_0 = q_0_val;
    p = p_val;

    k = tan(pi * B_r / (2 * B_s));
}


// NOTE::
    // -- -- --- //
    // ACCESSORS //
    // -- -- --- //


double Flatley::get_H_c() const{
    return H_c;
}


double Flatley::get_B_r() const{
    return B_r;
}


double Flatley::get_B_s() const{
    return B_s;
}


double Flatley::get_q_0() const{
    return q_0;
}


double Flatley::get_p() const{
    return p;
}

bool Flatley::get_slope_sign() const{
    return slope_sign;
}

void Flatley::get_parameters(double& H_c_val,
                             double& B_r_val,
                             double& B_s_val,
                             double& q_0_val,
                             double& p_val) const{
    H_c_val = H_c;
    B_r_val = B_r;
    B_s_val = B_s;
    q_0_val = q_0;
    p_val = p;
}

Vector Flatley::get_mag_field() const{
    return n_0 * B_0;
}

// Function to get new magnetic field
double Flatley::calc_mag_field(double dt, Vector H_new, Vector n_new){
    H_prev = H_0;
    H_0 = H_new;
    n_prev = n_0;
    n_0 = n_new;

    update_slope_sign();

    double H_mag = H_0 * n_0.direction();
    double Bd_H = H_mag - (H_prev * n_prev.direction());
    double theta = (pi * B_0) / (2 * B_s);
    double k_0 = tan(theta) / (k * H_c);
    double beta = 0;

    if (slope_sign == 0)
        beta = pow((H_mag - k_0 - H_c) / (2 * H_c), p);
    else
        beta = pow((H_mag - k_0 + H_c) / (2 * H_c), p);

    double alpha = (q_0 + (1 - q_0) * beta) * (2 * k * B_s / pi);
    double cos_2 = pow(cos(theta), 2);

    double dB_dt = (alpha * cos_2 * Bd_H / dt);

    double B_new = B_0 + dB_dt * dt;

    B_prev = B_0;
    B_0 = B_new;

    return B_new;
}


// NOTE::
    // -- -- -- --- //
    // CONSTRUCTORS //
    // -- -- -- --- //


// Default constructor
Flatley::Flatley()
    : H_0(), H_prev(), n_0(), n_prev()
{}


// Constructor with timestep and initial vectors
Flatley::Flatley(
    Vector H_init_0,
    Vector H_init_prev,
    Vector n_init_0,
    Vector n_init_prev
)
    : H_0(H_init_0), H_prev(H_init_prev),
      n_0(n_init_0), n_prev(n_init_prev)
{}


// Constructor with most specifications
Flatley::Flatley(const Vector& H_init_0,
                 const Vector& H_init_prev,
                 const Vector& n_init_0,
                 const Vector& n_init_prev,
                 double H_c_val,
                 double B_r_val,
                 double B_s_val,
                 double q_0_val,
                 double p_val)
    : H_c(H_c_val),
      B_r(B_r_val),
      B_s(B_s_val),
      q_0(q_0_val),
      p(p_val),
      k(tan(pi * B_r / (2 * B_s))/H_c_val),
      H_0(H_init_0),
      H_prev(H_init_prev),
      n_0(n_init_0),
      n_prev(n_init_prev),
      slope_sign(false)
{}
