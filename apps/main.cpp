#include <iostream>
#include <string>
#include "Vector.h"
#include "Matrix.h"
#include "DateTime.h"
#include "Simulation.h"
#include "Satellite.h"
#include "Shell.h"
using namespace std;

int main()
{
    // Clearing screen
    cout << clear_screen_string();

    /* NOTE:
        // -- -- -- -- -- -- -- -- - //
        // SIMULATION DATA n DETAILS //
        // -- -- -- -- -- -- -- -- - //
    */

    string inputmagfile = "../data/csv/igrf-icrf_55_10d-1s.csv";
    string outputcsvfile = "../results/shared_data.csv";
    DateTime start_time("01 Oct 2025 07:00:00.000");
    DateTime stop_time("01 Oct 2025 14:00:00.000");
    double timestep = 1;       // in seconds


    /* NOTE:
        // -- -- -- -- -- --- //
        // HYSTERESIS DETAILS //
        // -- -- -- -- -- --- //
    */

    // For PMAC reference paper
    double H_c = 12;            // Coercivity
    double B_r = 0.004;         // Retentivity
    double B_s = 0.027;         // Saturation
    double q_0 = 0;             // Required for defining flatley loop
    double p   = 2;             // Required for defining flatley loop


    /* NOTE:
        // -- -- -- -- -- - //
        // SATELITE DETAILS //
        // -- -- -- -- -- - //
    */

    Matrix moment_of_inertia = {
        0.0067, 0.0000, 0.0000,
        0.0003, 0.0333, 0.0000,
        0.0000, 0.0000, 0.0333
    };

    // Orientation and velicity
    Vector x = {1, 0, 0};       // Orientation of x axis
    Vector y = {0, 1, 0};       // Orientation of y axis
    Vector z = {0, 0, 1};       // Orientation of z axis
    Vector angular_velocity = {0.6, 0.6, 0.6};
    Vector angular_acceleration = {0.00001, 0, 0};

    // Data for magnetic moment
    double bar_m    = 12.0;     // Magnetic moment of bar magnet
    double hyst_vol = 7.4e-8;   // Volume of one hysteresis rod
    double hyst_nd  = 0;        // Demagnetising factor (~0 for L/D > 30 E < 2%)
    int num_x_hyst  = 3;        // --}
    int num_y_hyst  = 3;        // --}}--number of hysteresis rods
    int num_z_hyst  = 0;        // --}


    /* NOTE:
        // -- -- -- -- -- -- -- -- -- -- -- -- //
        // READING DATA n SIMULATING SATELLITE //
        // -- -- -- -- -- -- -- -- -- -- -- -- //
    */

    // Activating Virtual environment
    venv_activate();

    // Reading and storing magetic fields
    cout << "Reading Magnetic Field Data..." << endl;
    vector<MagneticEntry> mag_data;
    mag_data = read_mag_file( inputmagfile, start_time, stop_time);
    cout << "Read magnetic field data." << endl
                                        << endl;

    // Initialising Satellite
    cout << "Initialising Satellite..." << endl;
    Satellite ahan(moment_of_inertia,
                   x, y, z, angular_velocity, angular_acceleration,
                   bar_m, hyst_vol, hyst_nd,
                   num_x_hyst, num_y_hyst, num_z_hyst,
                   H_c, B_r, B_s, q_0, p);
    cout << "Initialised Satellite..." << endl
                                       << endl;
    // Exporting parameters
    export_params(ahan, outputcsvfile);

    // Simulating and storing data
    simulate(ahan, mag_data, timestep, outputcsvfile);

    // Plotting data
    command("python3 ../utils/plot.py " + outputcsvfile);

    // Closing Virtual environment
    venv_deactivate();

    return 0;
}
