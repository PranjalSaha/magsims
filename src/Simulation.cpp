#include "Simulation.h"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <sstream>
#include "Numerics.h"
#include "Vector.h"
#include "DateTime.h"
#include "Satellite.h"

using namespace std;

string progressBar(int current, int total, const string &label){
    const int bar_width = 50;
    float progress = static_cast<float>(current) / total;

    ostringstream buffer;
    buffer << left << setw(15) << label << " [";

    int pos = static_cast<int>(bar_width * progress);
    for (int i = 0; i < bar_width; ++i){
        if (i < pos)
            buffer << "=";
        else if (i == pos)
            buffer << ">";
        else
            buffer << " ";
    }

    buffer << "] ";
    buffer << setw(3) << int(progress * 100.0) << "% ";
    buffer << "(" << current << "/" << total << ")";
    buffer << "\r";

    return buffer.str();
}

string clearScreenString(){
#ifdef _WIN32
    // Windows does not support ANSI escape codes by default in old terminals
    return "cls";  // To be used with system("cls")
#else
    // ANSI escape sequence for clearing the screen and moving the cursor to top
    return "\033[2J\033[1;1H";
#endif
}

SampleDataVector readMagFile( const string& filename){
    SampleDataVector data(filename, "\"Time (UTCG)\"", "\"x (nT)\"",
                          "\"y (nT)\"","\"z (nT)\"");
    data = data * 7.95e-4;
    return data;
}

void exportParams(const Satellite& satellite,
                   const string& outputfile
){
    string satellite_info_filename = 
        outputfile.substr( 0, outputfile.find_last_of('.')
            ) + "_satellite_info.txt";
    ofstream info_file(satellite_info_filename);

    info_file << fixed << setprecision(6);
    info_file << "Satellite Initial Configuration\n";
    info_file << "--------------------------------\n\n";

    info_file << "Moment of Inertia:\n" 
              << satellite.displayMomentOfInertia() << "\n";

    info_file << "Initial Orientation Vectors:\n"
              << "< x \\n y \\n z >" << endl
              << satellite.displayOrientation() << "\n";

    info_file << "Initial Angular Velocity: "
              << satellite.displayAngularVelocity() << "\n";

    info_file << "Initial Angular Acceleration: "
              << satellite.displayAngularAcceleration() << "\n";

    info_file << "\nMagnetic Moment Details:\n";
    info_file << "Bar Magnet Moment (bar_m): " << satellite.getBarM() << "\n";
    info_file << "Hysteresis Rods:\n";
    info_file << "Volume: " << satellite.getHystVol() << " m^3\n";
    info_file << "Demag. Factor: " << satellite.getHystNd() << "\n";
    info_file << "Number of Rods - X: " << satellite.getNumXHyst()
              << ", Y: " << satellite.getNumYHyst()
              << ", Z: " << satellite.getNumZHyst() << "\n";

    info_file.close();
}

void printParams(const Satellite& satellite, ostream& ostring)
{
    ostring << fixed << setprecision(6);
    ostring << "Satellite Initial Configuration\n";
    ostring << "--------------------------------\n\n";

    ostring << "Moment of Inertia:\n"
       << satellite.displayMomentOfInertia() << "\n";

    ostring << "Initial Orientation Vectors:\n"
       << "< x \\n y \\n z >\n"
       << satellite.displayOrientation() << "\n";

    ostring << "Initial Angular Velocity: "
       << satellite.displayAngularVelocity() << "\n";

    ostring << "Initial Angular Acceleration: "
       << satellite.displayAngularAcceleration() << "\n";

    ostring << "\nMagnetic Moment Details:\n";
    ostring << "Bar Magnet Moment (bar_m): "
       << satellite.getBarM() << "\n";

    ostring << "Hysteresis Rods:\n";
    ostring << "Volume: "
       << satellite.getHystVol() << " m^3\n";
    ostring << "Demag. Factor: "
       << satellite.getHystNd() << "\n";
    ostring << "Number of Rods - X: "
       << satellite.getNumXHyst()
       << ", Y: " << satellite.getNumYHyst()
       << ", Z: " << satellite.getNumZHyst() << "\n";
}

string get_header(){
    // Just returns the header of the data file
    ostringstream header;
    header << "Time" << ","
           << "aux_mag_body_x(A/m)" << ","
           << "aux_mag_body_y(A/m)" << ","
           << "aux_mag_body_z(A/m)" << ","
           << "aux_mag_body_m(A/m)" << ","
           << "hys_mag_body_x(Tesla)" << ","
           << "hys_mag_body_y(Tesla)" << ","
           << "hys_mag_body_z(Tesla)" << ","
           << "hys_mag_body_m(Tesla)" << ","
           << "mag_mmt_body_x(Am2)" << ","
           << "mag_mmt_body_y(Am2)" << ","
           << "mag_mmt_body_z(Am2)" << ","
           << "mag_mmt_inrt_x(Am2)" << ","
           << "mag_mmt_inrt_y(Am2)" << ","
           << "mag_mmt_inrt_z(Am2)" << ","
           << "mag_mmt_m(Am2)" << ","
           << "torque_x_inrt(Nm)" << ","
           << "torque_y_inrt(Nm)" << ","
           << "torque_z_inrt(Nm)" << ","
           << "torque_x_body(Nm)" << ","
           << "torque_y_body(Nm)" << ","
           << "torque_z_body(Nm)" << ","
           << "ang_vel_inrt_x(rad/s)" << ","
           << "ang_vel_inrt_y(rad/s)" << ","
           << "ang_vel_inrt_z(rad/s)" << ","
           << "ang_vel_inrt_m(rad/s)" << ","
           << "ang_acc_inrt_x(rad/s2)" << ","
           << "ang_acc_inrt_y(rad/s2)" << ","
           << "ang_acc_inrt_z(rad/s2)" << ","
           << "ang_acc_inrt_m(rad/s2)" << ","
           << "ang_vel_body_x(rad/s)" << ","
           << "ang_vel_body_y(rad/s)" << ","
           << "ang_vel_body_z(rad/s)" << ","
           << "ang_vel_body_m(rad/s)" << ","
           << "ang_acc_body_x(rad/s2)" << ","
           << "ang_acc_body_y(rad/s2)" << ","
           << "ang_acc_body_z(rad/s2)" << ","
           << "ang_acc_body_m(rad/s2)" << endl;
    return header.str();
}

// ---------------------------------------------
// Simulation Definition
// ---------------------------------------------

SimulationContext::SimulationContext(const DateTime& t)
        : time(t),
          m{0,0,0},
          torque{0,0,0},
          trqBody{0,0,0},
          angularVelocity{0,0,0},
          angularAcceleration{0,0,0},
          hystMagField{0,0,0},
          orientation{{0,0,0},{0,0,0},{0,0,0}}
    {}

void advancePhysicsStep(Satellite& satellite,
                        SampleDataVector& mag_data,
                        SimulationContext& ctx,
                        double dt)
{
    Vector xBody = ctx.orientation[0];
    Vector yBody = ctx.orientation[1];
    Vector zBody = ctx.orientation[2];

    Vector H = mag_data.lagrangeInterpolate(ctx.time);

    satellite.updateHystM(H, dt);
    ctx.m = satellite.getHystM();

    ctx.torque = (ctx.m ^ H) * mu_0;

    ctx.trqBody = { ctx.torque * xBody,
                    ctx.torque * yBody,
                    ctx.torque * zBody };

    satellite.applyTorque(ctx.torque, dt);

    ctx.angularVelocity = satellite.getAngularVelocity();
    ctx.angularAcceleration =
        satellite.getAngularAcceleration();

    ctx.orientation = satellite.getOrientation();
    ctx.hystMagField = satellite.getHystB();
}

void integrateEuler(Satellite& satellite,
                    SampleDataVector& mag_data,
                    SimulationContext& ctx,
                    double dt){
    advancePhysicsStep(satellite, mag_data, ctx, dt);
}

void integrateRK4(Satellite& satellite,
                  SampleDataVector& mag_data,
                  SimulationContext& ctx,
                  double dt){
    Satellite s1 = satellite;
    Satellite s2 = satellite;
    Satellite s3 = satellite;
    Satellite s4 = satellite;

    SimulationContext c1 = ctx;
    SimulationContext c2 = ctx;
    SimulationContext c3 = ctx;
    SimulationContext c4 = ctx;

    advancePhysicsStep(s1, mag_data, c1, dt);
    advancePhysicsStep(s2, mag_data, c2, dt * 0.5);
    advancePhysicsStep(s3, mag_data, c3, dt * 0.5);
    advancePhysicsStep(s4, mag_data, c4, dt);

    satellite.setAngularVelocity(
        (c1.angularVelocity +
         c2.angularVelocity * 2.0 +
         c3.angularVelocity * 2.0 +
         c4.angularVelocity) / 6.0
    );

    satellite.setOrientation(
        (c1.orientation[0] +
         c2.orientation[0] * 2.0 +
         c3.orientation[0] * 2.0 +
         c4.orientation[0]) / 6.0,
        (c1.orientation[1] +
         c2.orientation[1] * 2.0 +
         c3.orientation[1] * 2.0 +
         c4.orientation[1]) / 6.0,
        (c1.orientation[2] +
         c2.orientation[2] * 2.0 +
         c3.orientation[2] * 2.0 +
         c4.orientation[2]) / 6.0
    );

    ctx.angularVelocity = satellite.getAngularVelocity();
    ctx.orientation     = satellite.getOrientation();
}

double computeAdaptiveTimestep(const SimulationContext& ctx,
                               double dtMin,
                               double dtMax)
{
    double omega = ctx.angularVelocity.magnitude();

    double dt = (omega * dtMin + dtMax) / (1 + omega);

    return dt;
}

void simulate(Satellite satellite,
              SampleDataVector mag_data,
              DateTime startTime,
              DateTime stopTime,
              double baseTimestep,
              string filename,
              IntegratorType integrator,
              bool adaptiveTimestep) {

    // Defining Variables for writing
    Vector m = {0,0,0},
           x_inrt = {1,0,0},
           y_inrt = {0,1,0},
           z_inrt = {0,0,1},
           x_body = {1,0,0},
           y_body = {0,1,0},
           z_body = {0,0,1},
           torque = {0,0,0},
           trq_bd = {0,0,0},
           angular_velocity = {0,0,0},
           angular_acceleration = {0,0,0},
           hyst_mag_field = {0,0,0},
           H = {0,0,0};

    double hyst_mag_x = 0.0,
           hyst_mag_y = 0.0,
           hyst_mag_z = 0.0,
           hyst_mag_m = 0.0;

    filename = filename.substr(0, filename.find_last_of('.'))
               + ".csv";
    ofstream fout(filename);
    fout << get_header();

    SimulationContext ctx(startTime);

    int duration = stopTime - startTime;

    while (ctx.time < stopTime)
    {
        // go up 13 lines
        // cout << "\033[13A\033[1G";

        // start printing from the 8th line
        cout << "\033[25;1H";

        double dt = baseTimestep;
        if (adaptiveTimestep) {
            dt = computeAdaptiveTimestep(ctx, 0.01, baseTimestep);
        }

        if (integrator == IntegratorType::Euler){
            integrateEuler(satellite, mag_data, ctx, dt);
        } else {
            integrateRK4(satellite, mag_data, ctx, dt);
        }

        // ---- File output (UNCHANGED LOGIC) ----

        // Getting Variable Values
        hyst_mag_field = satellite.getHystB();
        x_body = ctx.orientation[0];
        y_body = ctx.orientation[1];
        z_body = ctx.orientation[2];
        hyst_mag_x = hyst_mag_field * x_body;
        hyst_mag_y = hyst_mag_field * y_body;
        hyst_mag_z = hyst_mag_field * z_body;
        hyst_mag_m = hyst_mag_field.magnitude();
        angular_velocity = satellite.getAngularVelocity();
        angular_acceleration = satellite.getAngularAcceleration();
        torque = ctx.torque;
        trq_bd = ctx.trqBody;
        m = ctx.m;
        H = mag_data.linearInterpolate(ctx.time);

        // Writing to the file
        fout << ctx.time.display() << ","
             << H*x_body << ","
             << H*y_body << ","
             << H*z_body << ","
             << H.magnitude() << ","
             << hyst_mag_x << ","
             << hyst_mag_y << ","
             << hyst_mag_z << ","
             << hyst_mag_m << ","
             << m * x_body << ","
             << m * y_body << ","
             << m * z_body << ","
             << m[0] << ","
             << m[1] << ","
             << m[2] << ","
             << m.magnitude() << ","
             << torque * x_inrt << ","
             << torque * y_inrt << ","
             << torque * z_inrt << ","
             << torque * x_body << ","
             << torque * y_body << ","
             << torque * z_body << ","
             << angular_velocity[0] << ","
             << angular_velocity[1] << ","
             << angular_velocity[2] << ","
             << angular_velocity.magnitude() << ","
             << angular_velocity * x_body << ","
             << angular_velocity * y_body << ","
             << angular_velocity * z_body << ","
             << angular_velocity.magnitude() << ","
             << angular_acceleration[0] << ","
             << angular_acceleration[1] << ","
             << angular_acceleration[2] << ","
             << angular_acceleration * x_body << ","
             << angular_acceleration * y_body << ","
             << angular_acceleration * z_body << ","
             << angular_acceleration.magnitude() << endl;

        // ---- Screen output ----
        ostringstream buffer;

        buffer << "Time                         : " << ctx.time.display() << endl
               << "Auxiliary Magnetic Field     : " << H.display() << endl
               << "                 Magnitude   : " << H.magnitude() << endl
               << "Hysteresis Magnetic Field(T) : " << hyst_mag_field.display() 
                    << endl
               << "Torque - Inertial (Nm)       : " << torque.display() << endl
               << "Torque - Body (Nm)           : " << trq_bd.display() << endl
               << "                 Magnitude   : " << hyst_mag_m << endl
               << "Angular Acceleration (rad/s) : "
                    << angular_acceleration.display() << endl
               << "                 Magnitude   : "
                    << angular_acceleration.magnitude() << endl
               << "Angular Velocity (Body)      : " <<
                    Vector{angular_velocity * x_body, 
                           angular_velocity * y_body, 
                           angular_velocity * z_body}.display() << endl
               << "Angular Acceleration (Body)  : " <<
                    Vector{angular_acceleration * x_body, 
                           angular_acceleration * y_body, 
                           angular_acceleration * z_body}.display() << endl;

        // add progress bar and write to screen
        int progress = ctx.time - startTime;
        buffer << progressBar(progress, duration, "Simulating");
        buffer << endl << endl;
        cout << buffer.str();
        ctx.time = ctx.time + dt;
    }
}

/*
void simulate(Satellite satellite,
              SampleDataVector mag_data,
              DateTime startTime,
              DateTime stopTime,
              double timestep,
              string filename){
    Vector m = {0,0,0},
           x_inrt = {1,0,0},
           y_inrt = {0,1,0},
           z_inrt = {0,0,1},
           x_body = {0,0,0},
           y_body = {0,0,0},
           z_body = {0,0,0},
           torque = {0,0,0},
           trq_bd = {0,0,0},
           angular_velocity = {0,0,0},
           angular_acceleration = {0,0,0},
           hyst_mag_field = {0,0,0};

    double hyst_mag_x = 0.0,
           hyst_mag_y = 0.0,
           hyst_mag_z = 0.0,
           hyst_mag_m = 0.0;

    vector<Vector> orientation = {{0,0,0}, {0,0,0}, {0,0,0}};

    // creating file objedt
    filename = filename.substr( 0, filename.find_last_of('.')) 
        + ".csv";
    ofstream fout(filename);

    // creating header
    fout << get_header();


    DateTime time = startTime;
    int duration = stopTime - startTime;
    while (time < stopTime){
        // printing at one particular place
        // cout << "\033[10;1H";
        cout << "\033[13A\033[1G";

        // Simulating one timestep
        Vector H = mag_data.lagrangeInterpolate(time);

        satellite.updateHystM(H, timestep);
        m = satellite.getHystM();
        torque = m ^ H * mu_0;
        trq_bd = {torque*x_body, torque*y_body, torque*z_body,};

        orientation = satellite.getOrientation();
        x_body = orientation[0];
        y_body = orientation[1];
        z_body = orientation[2];

        hyst_mag_field = satellite.getHystB();
        hyst_mag_x = hyst_mag_field * x_body;
        hyst_mag_y = hyst_mag_field * y_body;
        hyst_mag_z = hyst_mag_field * z_body;
        hyst_mag_m = hyst_mag_field.magnitude();

        satellite.applyTorque(torque, timestep);

        angular_velocity = satellite.getAngularVelocity();
        angular_acceleration = satellite.getAngularAcceleration();

        // Writing to the file
        fout << time.display() << ","
             << H*x_body << ","
             << H*y_body << ","
             << H*z_body << ","
             << H.magnitude() << ","
             << hyst_mag_x << ","
             << hyst_mag_y << ","
             << hyst_mag_z << ","
             << hyst_mag_m << ","
             << m * x_body << ","
             << m * y_body << ","
             << m * z_body << ","
             << m[0] << ","
             << m[1] << ","
             << m[2] << ","
             << m.magnitude() << ","
             << torque * x_inrt << ","
             << torque * y_inrt << ","
             << torque * z_inrt << ","
             << torque * x_body << ","
             << torque * y_body << ","
             << torque * z_body << ","
             << angular_velocity[0] << ","
             << angular_velocity[1] << ","
             << angular_velocity[2] << ","
             << angular_velocity * x_body << ","
             << angular_velocity * y_body << ","
             << angular_velocity * z_body << ","
             << angular_velocity.magnitude() << ","
             << angular_acceleration[0] << ","
             << angular_acceleration[1] << ","
             << angular_acceleration[2] << ","
             << angular_acceleration * x_body << ","
             << angular_acceleration * y_body << ","
             << angular_acceleration * z_body << ","
             << angular_acceleration.magnitude() << endl;

        // Use buffer for formatted output
        ostringstream buffer;

        buffer << "Time                         : " << time.display() << endl
               << "Auxiliary Magnetic Field     : " << H.display() << endl
               << "                 Magnitude   : " << H.magnitude() << endl
               << "Hysteresis Magnetic Field(T) : " << hyst_mag_field.display() 
                    << endl
               << "Torque - Inertial (Nm)       : " << torque.display() << endl
               << "Torque - Body (Nm)           : " << trq_bd.display() << endl
               << "                 Magnitude   : " << hyst_mag_m << endl
               << "Angular Acceleration (rad/s) : "
                    << angular_acceleration.display() << endl
               << "                 Magnitude   : "
                    << angular_acceleration.magnitude() << endl
               << "Angular Velocity (Body)      : " <<
                    Vector{angular_velocity * x_body, 
                           angular_velocity * y_body, 
                           angular_velocity * z_body}.display() << endl
               << "Angular Acceleration (Body)  : " <<
                    Vector{angular_acceleration * x_body, 
                           angular_acceleration * y_body, 
                           angular_acceleration * z_body}.display() << endl;

        // add progress bar and write to screen
        int progress = time - startTime;
        buffer << progressBar(progress, duration, "Simulating");
        buffer << endl << endl;
        cout << buffer.str();

        time = time + timestep;
    }
}
*/
