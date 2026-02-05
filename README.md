# Magnetic Satellite Simulation (MagSims)

A C++ simulation framework for modeling the attitude dynamics of satellites under the influence of magnetic fields. The core simulation calculates how magnetic torque affects satellite rotation, incorporating both permanent magnets and hysteresis rods using the Flatley hysteresis model.

## Features

- **Physics-Based Simulation**: Models passive magnetic attitude control using
permanent magnets and hysteresis rods
- **Flatley Hysteresis Model**: Realistic magnetic hysteresis behavior with
configurable parameters
- **3D Dynamics**: Full 3-DOF rotational dynamics with moment of inertia
- **Multiple Applications**: Includes standard simulations, debug scenarios, and
standalone component tests
- **Data Visualization**: Python plotting utilities for analyzing results

## Dependencies

### C++ Build Requirements
- **C++17** compatible compiler (GCC, Clang, or MSVC)
- **CMake 3.10+**

### Python Visualization (Optional)
- Python 3.x
- pandas
- matplotlib
- numpy

Install Python dependencies:
```bash
pip install pandas matplotlib numpy
```

## Building the Project

```bash
# Configure and build
cmake -S . -B build
cmake --build build
```

Executables are generated in the `bin/` directory with `.out` extension.

### Clean Build
```bash
rm -rf build/ bin/*.out
```

## Running Simulations

All executables should be run from the project root so relative paths work correctly:

```bash
# Standard simulation with STK magnetic field data
./bin/main.out

# Debug simulation with synthetic sinusoidal magnetic field
./bin/aligned_spin.out

# Standalone Flatley hysteresis model test
./bin/flatley_trial.out
```

## Project Structure

```
magsims/
├── include/          # Header files
│   ├── Vector.h      # 3D vector operations
│   ├── Matrix.h      # 3x3 matrix operations
│   ├── DateTime.h    # Time handling
│   ├── Flatley.h     # Flatley hysteresis model
│   ├── Satellite.h   # Satellite state and dynamics
│   └── Simulation.h  # Main simulation framework
├── src/              # Implementation files
├── apps/             # Application entry points
│   ├── main.cpp
│   ├── aligned_spin.cpp
│   └── flatley_trial.cpp
├── bin/              # Built executables (generated)
├── build/            # CMake build artifacts (generated)
├── data/csv/         # Input magnetic field data
├── results/          # Output CSV files and plots
├── utils/            # Python plotting scripts
│   ├── plot.py
│   └── plot_hyst_curve.py
└── references/       # Research papers and documentation
```

## Core Components

### Mathematical Foundation
- **Vector**: 3D vector operations with operator overloading for scalar
multiplication, dot product, cross product
- **Matrix**: 3x3 matrix operations for moment of inertia and coordinate
transformations
- **DateTime**: Time handling using `std::chrono` for parsing magnetic field data

### Physics Models
- **Flatley**: Implements the Flatley hysteresis model with parameters H_c
(coercivity), B_r (retentivity), B_s (saturation)
- **Satellite**: Complete satellite state including moment of inertia,
orientation, angular velocity, permanent magnets, and hysteresis rods
- **Simulation**: Main time-stepping loop with magnetic field transformation,
torque calculation, and data export

## Usage Example

```cpp
#include "Vector.h"
#include "Matrix.h"
#include "Satellite.h"
#include "Simulation.h"

// Define hysteresis parameters
double H_c = 3.0, B_r = 0.5, B_s = 1.5, q_0 = 0.05, p = 1.0;

// Define satellite properties
Matrix moment_of_inertia = {{0.01, 0, 0}, {0, 0.02, 0}, {0, 0, 0.015}};
Vector orientation_x = {1, 0, 0};
Vector orientation_y = {0, 1, 0};
Vector orientation_z = {0, 0, 1};
Vector angular_velocity = {0, 0, 0.1};
Vector bar_m = {0, 0, 1.0};  // Permanent magnet moment

// Load magnetic field data
vector<MagneticEntry> mag_data = read_mag_file("data/csv/magnetic_field.csv");

// Initialize and run simulation
Satellite sat(moment_of_inertia, orientation_x, orientation_y, orientation_z,
              angular_velocity, bar_m, /* hysteresis parameters... */);
simulate(sat, mag_data, timestep, "results/output.csv");
```

## Coordinate Frames

- **Inertial Frame**: Fixed reference frame (typically ECI - Earth-Centered
Inertial)
- **Body Frame**: Satellite-fixed frame defined by orthonormal orientation
vectors
- Magnetic field data is transformed from inertial to body frame during
simulation

## Data Format

### Input: Magnetic Field CSV
```
DateTime,B_x(nT),B_y(nT),B_z(nT)
2024-01-01 00:00:00,1000.5,2000.3,-500.7
...
```

### Output: Simulation Results CSV
Includes timestamps, magnetic field components, angular velocity, torque, and
magnetization data.

## Visualization

Python scripts are automatically called by the C++ code to generate plots:

```bash
# Manually plot simulation results
python3 utils/plot.py results/shared_data.csv

# Plot hysteresis curves
python3 utils/plot_hyst_curve.py results/flatley_trial_f1.csv
```

## Physics Background

The simulation implements passive magnetic attitude control (PMAC) using:
- **Permanent Magnets**: Provide alignment torque with the magnetic field
- **Hysteresis Rods**: Dissipate rotational energy through magnetic hysteresis
- **Flatley Model**: Accurately represents hysteresis behavior for realistic
damping

See `references/` directory for research papers on the Flatley model and PMAC
systems.

## Adding New Applications

1. Create a new file in `apps/` (e.g., `apps/custom_sim.cpp`)
2. Include necessary headers and implement `main()`
3. CMake will automatically discover and build it as `bin/custom_sim.out`

## Known Issues

- Line 139 in `Flatley.cpp` contains Python syntax that needs C++ conversion
- Large timesteps may cause numerical instability
- No explicit normalization of orientation vectors during integration

## References

- Flatley, T. W. "A Magnetic Hysteresis Model"
(see `references/flatleymodel.pdf`)
- Passive Magnetic Attitude Control papers in `references/`

## Contributing

When modifying the code:
- Follow the existing code structure and naming conventions
- Test with both synthetic and real magnetic field data
- Verify numerical stability with various timesteps
- Update relevant documentation

## License

See LICENSE.md for licensing information.
