#ifndef SIMULATION_H
#define SIMULATION_H

#include "Satellite.h"
#include "Numerics.h"

// Function to display progress bar
string progressBar(int current, int total, const string &label);

string clearScreenString();

SampleDataVector readMagFile(const string& filename);

// Function to export details
void exportParams(const Satellite& satellite, 
                  const string& outputCsvFilename);
void printParams(const Satellite& satellite, ostream& ostring);

enum class IntegratorType
{
    Euler,
    RungeKutta4
};

struct SimulationContext
{
    DateTime time;

    Vector m;
    Vector torque;
    Vector trqBody;

    Vector angularVelocity;
    Vector angularAcceleration;

    Vector hystMagField;

    std::vector<Vector> orientation;

    explicit SimulationContext(const DateTime& t);
};

// Function to simulate a satellite
void simulate(Satellite satellite,
              SampleDataVector mag_data,
              DateTime startTime,
              DateTime stopTime,
              double baseTimestep,
              std::string filename,
              IntegratorType integrator,
              bool adaptiveTimestep);

#endif
