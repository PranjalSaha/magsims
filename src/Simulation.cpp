#include <iostream>
#include <iomanip>
#include <string>
#include <fstream>
#include <sstream>
#include <stdexcept>
#include "Simulation.h"
#include "Debug.h"
#include "Vector.h"
#include "DateTime.h"
#include "Satellite.h"

using namespace std;

string progress_bar(int current, int total, const string &label){
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

string clear_screen_string(){
#ifdef _WIN32
    // Windows does not support ANSI escape codes by default in old terminals
    return "cls";  // To be used with system("cls")
#else
    // ANSI escape sequence for clearing the screen and moving the cursor to top
    return "\033[2J\033[1;1H";
#endif
}

vector<MagneticEntry> read_mag_file(
    const string& filename,
    const DateTime& start_time,
    const DateTime& end_time
){
    vector<MagneticEntry> data;
    ifstream file(filename);

    if (!file.is_open()){
        throw runtime_error(
            "Failed to open file: " + filename + "\n" + 
            CallStackTracker::trace()
        );
    }

    string line;
    getline(file, line);  // Skip header

    while (getline(file, line)){
        istringstream ss(line);
        string datetime_str;
        getline(ss, datetime_str, ',');  // Extract date/time

        DateTime current_time(datetime_str);

        if (current_time < start_time)
        {
            continue;
        }

        if (current_time > end_time)
        {
            break;
        }

        string x_str, y_str, z_str;
        getline(ss, x_str, ',');
        getline(ss, y_str, ',');
        getline(ss, z_str, ',');

        // converting to auxiliary magnetic field (SI Units A/m)
        double x = stod(x_str) * (7.958E-4);
        double y = stod(y_str) * (7.958E-4);
        double z = stod(z_str) * (7.958E-4);
        Vector H = {x, y, z};

        data.emplace_back(current_time, H);
    }

    file.close();
    return data;
}


vector<MagneticEntry> read_mag_file(
    const string& filename,
    const string& start_time_str,
    const string& end_time_str
){
    DateTime start_time(start_time_str);
    DateTime end_time(end_time_str);

    return read_mag_file(filename, start_time, end_time);
}


void export_params(const Satellite& satellite,
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
              << satellite.display_moment_of_inertia() << "\n";

    info_file << "Initial Orientation Vectors:\n"
              << "< x \\n y \\n z >" << endl
              << satellite.display_orientation() << "\n";

    info_file << "Initial Angular Velocity: "
              << satellite.display_angular_velocity() << "\n";

    info_file << "Initial Angular Acceleration: "
              << satellite.display_angular_acceleration() << "\n";

    info_file << "\nMagnetic Moment Details:\n";
    info_file << "Bar Magnet Moment (bar_m): " << satellite.get_bar_m() << "\n";
    info_file << "Hysteresis Rods:\n";
    info_file << "Volume: " << satellite.get_hyst_vol() << " m^3\n";
    info_file << "Demag. Factor: " << satellite.get_hyst_nd() << "\n";
    info_file << "Number of Rods - X: " << satellite.get_num_x_hyst()
              << ", Y: " << satellite.get_num_y_hyst()
              << ", Z: " << satellite.get_num_z_hyst() << "\n";

    info_file.close();
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

void simulate(Satellite satellite,
              vector<MagneticEntry> mag_data,
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
    fout << "Time" << ","
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
         << "ang_vel_body_x(rad/s)" << ","
         << "ang_vel_body_y(rad/s)" << ","
         << "ang_vel_body_z(rad/s)" << ","
         << "ang_vel_body_m(rad/s)" << ","
         << "ang_acc_inrt_x(rad/s2)" << ","
         << "ang_acc_inrt_y(rad/s2)" << ","
         << "ang_acc_inrt_z(rad/s2)" << ","
         << "ang_acc_body_x(rad/s2)" << ","
         << "ang_acc_body_y(rad/s2)" << ","
         << "ang_acc_body_z(rad/s2)" << ","
         << "ang_acc_body_m(rad/s2)" << endl;


    for (int i = 0; i < mag_data.size(); i++){
        // printing at one particular place
        cout << "\033[10;1H";

        // Simulating one timestep
        auto [time, H] = mag_data[i];

        satellite.update_hyst_m(H, timestep);
        m = satellite.get_net_m();
        torque = m ^ H * mu_0;
        trq_bd = {torque*x_body, torque*y_body, torque*z_body,};

        orientation = satellite.get_orientation();
        x_body = orientation[0];
        y_body = orientation[1];
        z_body = orientation[2];

        hyst_mag_field = satellite.get_hyst_b();
        hyst_mag_x = hyst_mag_field * x_body;
        hyst_mag_y = hyst_mag_field * y_body;
        hyst_mag_z = hyst_mag_field * z_body;
        hyst_mag_m = hyst_mag_field.magnitude();

        satellite.apply_torque(torque, timestep);

        angular_velocity = satellite.get_angular_velocity();
        angular_acceleration = satellite.get_angular_acceleration();

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
        buffer << progress_bar(i+1, mag_data.size(), "Simulating");
        buffer << endl << endl;
        cout << buffer.str();
    }
}
