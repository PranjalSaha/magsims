cmake_minimum_required(VERSION 3.10)

# Project definition
project(Magnetic_Simulation LANGUAGES CXX)

# Set C++ standard (C++17)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED OFF)

# Export compile_commands.json (useful for IDEs, static analyzers, clangd)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------------------
# Build the common library
# --------------------------------------------------------------

# Collect all .cpp files in src/ (implementation files, no main())
file(GLOB SRC_FILES src/*.cpp)

# Create a library target with those files
add_library(myproject_lib ${SRC_FILES})

# Add include/ as a public header search path
target_include_directories(myproject_lib PUBLIC include)

# --------------------------------------------------------------
# Build executables from apps/
# --------------------------------------------------------------

# Collect all application sources (each has its own main())
file(GLOB APPS ${CMAKE_SOURCE_DIR}/apps/*.cpp)

# Create one executable per file in apps/
foreach(APP_FILE ${APPS})
    # Extract the filename without extension (e.g., main1.cpp -> main1)
    get_filename_component(EXE_NAME ${APP_FILE} NAME_WE)

    # Append ".out" to the executable name
    set(EXE_NAME "${EXE_NAME}.out")

    # Define the executable
    add_executable(${EXE_NAME} ${APP_FILE})

    # Link with the shared project library
    target_link_libraries(${EXE_NAME} PRIVATE myproject_lib)

    # Place executables in ../bin/
    set_target_properties(${EXE_NAME} PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}/bin
    )
endforeach()
