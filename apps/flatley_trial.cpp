#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cmath>
#include "Flatley.h"
#include "Vector.h"
#include "Simulation.h"
using namespace std;

int main()
{
    // Clearing screen
    // cout << clear_screen_string();

    /* NOTE:
        // -- -- -- -- -- -- -- -- - //
        // SIMULATION DATA n DETAILS //
        // -- -- -- -- -- -- -- -- - //
    */

    string outputfile = "../results/flatley_trial.csv";
    ofstream file(outputfile);
    double timestep = 3;       // in seconds


    /* NOTE:
        // -- -- -- -- -- --- //
        // HYSTERESIS DETAILS //
        // -- -- -- -- -- --- //
    */

    // For soft iron core
    double H_c = 12;            // Coercivity
    double B_r = 0.004;         // Retentivity
    double B_s = 0.027;         // Saturation
    double q_0 = 0;             // Required for defining flatley loop
    double p   = 2;             // Required for defining flatley loop

    // Defining the flatley rod
    Flatley rod(
        {0.001, 0, 0}, {0.002, 0, 0},
        {1, 0, 0}, {1, 0, 0},
        H_c, B_r, B_s, q_0, p
    );

    /* NOTE:
        // -- -- -- -- -- //
        // MAGNETIC FIELD //
        // -- -- -- -- -- //
    */

    double freq = 0.001;        // hertz
    double amp = 100;           // Gauss
    int num_iterations = 1000000;

    Vector B;       // Magnetic Field
    Vector H;       // Auxiliary Field
    Vector x = {1, 0, 0};
    bool slope = 0;

    file << "S.No,H(A/m),B(T),slope" << endl;

    for (int counter = 0; counter < num_iterations; counter++){

        H = {amp * sin(freq * counter * timestep), 0, 0};
        rod.calc_mag_field(timestep, H, {1, 0, 0});
        B = rod.get_mag_field();
        slope = rod.get_slope_sign();

        file << counter+1 << ","
             << H*x << "," 
             << B*x << ","
             << slope << endl;

        ostringstream output;
        output << progress_bar(counter, num_iterations, "Simulation Progress");
        cout << output.str();
    }

    /* NOTE:
        // -- -- -- -- -- -- -- -- -- -- -- -- //
        // READING DATA n SIMULATING SATELLITE //
        // -- -- -- -- -- -- -- -- -- -- -- -- //
    */

    // Plotting data
    string command = "python3 ../utils/plot_hyst_curve.py " + outputfile;
    int result = system(command.c_str());
    if (result != 0)
        cerr << "Error: Python script failed with code " << result << "\n";

    return 0;
}
