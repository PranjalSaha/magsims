# WARP.md

This file provides guidance to WARP (warp.dev) when working with code in this repository.

## Project Overview
This is a C++ magnetic satellite simulation project that models the attitude dynamics of satellites under the influence of magnetic fields. The core simulation calculates how magnetic torque affects satellite rotation, incorporating both permanent magnets and hysteresis rods using the Flatley hysteresis model.

## Build System

### Building the Project
```bash
# From the project root, configure and build
cmake -S . -B build
cmake --build build
```

This creates executables in the `bin/` directory with `.out` extension (e.g., `main.out`, `aligned_spin.out`, `flatley_trial.out`).

### Running Executables
```bash
# Run from project root so relative paths work correctly
./bin/main.out
./bin/aligned_spin.out
./bin/flatley_trial.out
```

### Cleaning Build
```bash
rm -rf build/ bin/*.out
```

## Code Architecture

### Core Components

#### Mathematical Foundation
- **Vector** (`Vector.h/cpp`): 3D vector operations with overloaded operators for scalar multiplication, dot product (operator*), cross product (operator^), addition/subtraction
- **Matrix** (`Matrix.h/cpp`): 3x3 matrix operations for moment of inertia and coordinate transforms. Supports matrix-vector and matrix-matrix multiplication, determinant, transpose, inverse
- **DateTime** (`DateTime.h/cpp`): Time handling using `std::chrono` for parsing STK magnetic field data

#### Physics Models
- **Flatley** (`Flatley.h/cpp`): Implements the Flatley hysteresis model for magnetic materials
  - Models magnetic hysteresis using parameters: H_c (coercivity), B_r (retentivity), B_s (saturation), q_0, p
  - Tracks 5-point history (H_0 through H_4) for numerical differentiation
  - Maintains slope sign to determine ascending/descending branch of hysteresis curve
  - **KNOWN ISSUE**: Line 139 in `Flatley.cpp` contains Python syntax (`for i in range(num_substeps):`) that needs to be converted to C++

- **Satellite** (`Satellite.h/cpp`): Complete satellite state representation
  - Moment of inertia (3x3 matrix)
  - Orientation (x, y, z body frame vectors in inertial frame)
  - Angular velocity and acceleration
  - Permanent bar magnet (bar_m, bar_dir)
  - Hysteresis rods in x, y, z directions (each using Flatley model)
  - `apply_torque()`: Integrates torque to update angular motion
  - `update_hyst_m()`: Updates magnetic moment of hysteresis rods

#### Simulation Framework
- **Simulation** (`Simulation.h/cpp`): Main simulation loop and I/O
  - `read_mag_file()`: Parses CSV files with magnetic field data (converts nT to A/m)
  - `simulate()`: Main time-stepping loop that:
    1. Transforms magnetic field from inertial to body frame
    2. Updates hysteresis rod magnetization
    3. Calculates magnetic torque (m × B)
    4. Applies torque to update satellite attitude
    5. Exports data to CSV
  - `export_params()`: Saves satellite configuration to text file
  - `progress_bar()`: Console progress indicator

### Applications Structure
Each `.cpp` file in `apps/` creates a separate executable:
- **main.cpp**: Standard simulation using STK magnetic field data from CSV files
- **aligned_spin.cpp**: Debug simulation with synthetic sinusoidal magnetic field for controlled testing
- **flatley_trial.cpp**: Standalone test for Flatley hysteresis model, generates H-B curves

Applications follow this pattern:
1. Define simulation parameters (time range, timestep)
2. Define hysteresis parameters (H_c, B_r, B_s, q_0, p)
3. Define satellite physical properties (MOI, orientation, angular velocity, magnetic moments)
4. Load or generate magnetic field data
5. Initialize Satellite object
6. Call `simulate()` to run time integration
7. Call Python plotting scripts to visualize results

### Data Flow
1. Magnetic field data (CSV format: DateTime, B_x, B_y, B_z) → `data/csv/`
2. Simulation reads data with `read_mag_file()`, converts nT to A/m
3. Main loop in `simulate()` steps through time
4. Results exported to CSV → `results/`
5. Python scripts generate plots saved as `.pkl` files

### Coordinate Frames
- **Inertial Frame**: Fixed reference frame (typically ECI)
- **Body Frame**: Satellite-fixed frame defined by x, y, z orientation vectors
- Magnetic field data is typically in inertial frame and must be transformed to body frame using the orientation vectors

## Python Utilities

### Environment Setup
```bash
# Activate virtual environment (if needed)
source venv/bin/activate

# Deactivate when done
deactivate
```

### Plotting Scripts
All Python scripts are in `utils/` and expect to be called from project root:

```bash
# Plot simulation results (magnetic field, angular velocity, torque, etc.)
python3 utils/plot.py results/shared_data.csv

# Plot hysteresis curve (H vs B)
python3 utils/plot_hyst_curve.py results/flatley_trial_f1.csv
```

The C++ code automatically calls these scripts via the `Shell.h` `command()` function.

## File Organization
- `include/` - All header files
- `src/` - Implementation files (no main())
- `apps/` - Application entry points (each has main())
- `bin/` - Built executables (generated by CMake)
- `build/` - CMake build artifacts
- `data/csv/` - Input magnetic field data
- `results/` - Output CSV files and info files
- `utils/` - Python plotting and analysis scripts
- `references/` - Research papers (PMAC.pdf, flatleymodel.pdf, etc.)

## Common Development Patterns

### Adding a New Application
1. Create `apps/new_app.cpp` with a `main()` function
2. Include necessary headers: `Vector.h`, `Satellite.h`, `Simulation.h`, etc.
3. CMake automatically discovers and builds it as `bin/new_app.out`

### Modifying Physics Parameters
Key parameters are typically defined near the top of application files:
- Hysteresis: H_c, B_r, B_s, q_0, p (from Flatley model references)
- Satellite geometry: moment_of_inertia (3x3 matrix)
- Magnetic moments: bar_m (Am²), hyst_vol (m³), num_x/y/z_hyst (count)

### Creating New Simulations
When creating custom magnetic field scenarios (like `aligned_spin.cpp`):
```cpp
vector<MagneticEntry> mag_data;
DateTime now = start_time;
for (int i = 0; i < num_iterations; i++) {
    Vector B = {/* your field components */};
    mag_data.emplace_back(now + i * timestep, B);
}
```

## Numerical Considerations
- The Flatley model uses first-order finite differences for dH/dt
- Timestep typically 1-10 seconds depending on dynamics
- Substeps may be needed for hysteresis integration (see line 137-138 in Flatley.cpp)
- Orientation vectors should remain orthonormal (no explicit normalization in current code)

## Known Issues
1. `Flatley.cpp` line 139 contains Python syntax that won't compile
2. Numerical instability possible with large timesteps or fast field variations
3. No validation that orientation vectors remain orthonormal during integration
4. Large output files may cause plotting scripts to sample data (MAX_ROWS_TO_READ = 10000)

## Physics Background
The simulation implements passive magnetic attitude control using:
- Permanent magnets for aligning with field
- Hysteresis rods for energy dissipation
- Flatley model for realistic hysteresis behavior (see `references/flatleymodel.pdf`)
- Reference papers in `references/` (PMAC.pdf, AD0271383.pdf)

## Dependencies
- **C++17** (set in CMakeLists.txt)
- **CMake 3.10+**
- **Python 3** with: pandas, matplotlib, numpy, pickle (for plotting)
- No external C++ libraries required
