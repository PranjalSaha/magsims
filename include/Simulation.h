#ifndef SIMULATION_H
#define SIMULATION_H

#include <string>
#include "Vector.h"
#include "DateTime.h"
#include "Satellite.h"
#include <vector>
#include <string>
#include <tuple>
#include "DateTime.h"
using namespace std;

using MagneticEntry = tuple<DateTime, Vector>;

// This is just a list of function definitions
// They are not very useful for the time being

// Function to display progress bar
string progress_bar(int current, int total, const string &label);

string clear_screen_string();

// Function to read all magnetic field and time from STK file
// nT to A/m conversion
vector<MagneticEntry> read_mag_file(
    const string& filename,
    const DateTime& start_time,
    const DateTime& end_time
);

// Overloaded function to read data from file
vector<MagneticEntry> read_mag_file(
    const string& filename,
    const string& start_time_str,
    const string& end_time_str
);

// Function to export details
void export_params(const Satellite& satellite, 
                    const string& output_csv_filename);

// Function to simulate a satellite
void simulate(Satellite satellite,
              vector<MagneticEntry> mag_data,
              double timestep,
              string filename);

#endif
