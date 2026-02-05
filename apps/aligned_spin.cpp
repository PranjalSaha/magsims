#include <iostream>
#include <string>
#include <filesystem>
#include "Vector.h"
#include "Matrix.h"
#include "DateTime.h"
#include "Simulation.h"
#include "Satellite.h"
#include "Shell.h"
#include "Numerics.h"
using namespace std;

int main()
{
    // Clearing screen
    cout << clearScreenString();

    /* NOTE:
        // -- -- -- -- -- -- -- -- - //
        // SIMULATION DATA n DETAILS //
        // -- -- -- -- -- -- -- -- - //
    */

    /*
    // Setting filenames
    string filename;
    cout << "Enter name of simulation (without extensions) : ";
    cin >> filename;
    cout << endl;
    string outputCSVFile = "../results/" + filename + ".csv";

    // Ensuring no overwrite
    bool writePermission = filesystem::exists(outputCSVFile);
    while (writePermission){
        cout << "File exists. Do you want to overwrite? [0/1]";
        cin >> writePermission;
        if (!writePermission){
            cout << "Enter name of simulation (without extensions) : ";
            cin >> filename;
            string outputCSVFile = "../results/" + filename + ".csv";
            writePermission = filesystem::exists(filename);
        }
    }

    cout << "Writing in to CSV file : " << outputCSVFile << endl;
    */

    // Setting filenames
    string filename;
    string outputCSVFile;

    cout << "\n========================================\n";
    cout << " Simulation Output Configuration\n";
    cout << "========================================\n\n";

    cout << "Enter simulation name (without extension): ";
    cin >> filename;

    outputCSVFile = "../results/" + filename + ".csv";

    // Ensuring no overwrite
    while (filesystem::exists(outputCSVFile)) {
        cout << "\n⚠️  File already exists:\n";
        cout << "   " << outputCSVFile << "\n\n";
        cout << "Overwrite file? [1 = Yes, 0 = No]: ";

        int overwrite;
        cin >> overwrite;

        if (overwrite == 1) {
            break;
        }

        cout << "\nEnter a new simulation name: ";
        cin >> filename;
        outputCSVFile = "../results/" + filename + ".csv";
    }

    cout << "\n----------------------------------------\n";
    cout << "Writing output to:\n";
    cout << "  " << outputCSVFile << "\n";
    cout << "----------------------------------------\n\n";

    // Inputting data
    string inputmagfile = "../data/csv/igrf-icrf_55_10d-1s.csv";

    // Setting simulation time details
    DateTime start_time("01 Oct 2025 07:00:00.000");
    DateTime stop_time("01 Oct 2025 23:59:59.000");
    double timestep = 1;       // in seconds


    /* NOTE:
        // -- -- -- -- -- --- //
        // HYSTERESIS DETAILS //
        // -- -- -- -- -- --- //
    */

    /*
    // From PMAC reference paper
    double H_c = 1.3381;        // Coercivity
    double B_r = 6.0618e-4;     // Retentivity
    double B_s = 0.300;         // Saturation
    double q_0 = 0;             // Required for defining flatley loop
    double p   = 2;             // Required for defining flatley loop
    */

    // HyMu80 values
    double H_c = 1.59154;       // Coercivity
    double B_r = 0.35;          // Retentivity
    double B_s = 8000;          // Saturation
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
    Vector angular_velocity = {0.17, 0.17, 0.17};
    Vector angular_acceleration = {0.00001, 0, 0};

    // Data for magnetic moment
    double bar_m    = 0000;     // Magnetic moment of bar magnet
    double hyst_vol = 18.4e-8;   // Volume of one hysteresis rod
    double hyst_nd  = 0;        // Demagnetising factor (~0 for L/D > 30 E < 2%)
    int num_x_hyst  = 9;        // --}
    int num_y_hyst  = 9;        // --}}--number of hysteresis rods
    int num_z_hyst  = 0;        // --}


    /* NOTE:
        // -- -- -- -- -- -- -- -- -- -- -- -- //
        // READING DATA n SIMULATING SATELLITE //
        // -- -- -- -- -- -- -- -- -- -- -- -- //
    */

    // Activating Virtual environment
    // venv_activate();

    // Reading and storing magetic fields
    cout << "Reading Magnetic Field Data..." << endl;
    SampleDataVector magData;
    for (int i = 1; i < (start_time - stop_time); i++){
        double A = 1.5;
        Vector B = {0, A, 0};
        magData.addSample(start_time + i, B);
    }
    magData.sort();
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
    exportParams(ahan, outputCSVFile);

    // Simulating and storing data
    // simulate(ahan, magData, start_time, stop_time, timestep, outputCSVFile);
    simulate(ahan, magData, start_time, stop_time, 1, outputCSVFile,
             IntegratorType::Euler, true);

    // Plotting data
    command("python3 ../utils/plot.py " + outputCSVFile);

    // Closing Virtual environment
    // venv_deactivate();

    return 0;
}
