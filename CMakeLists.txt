cmake_minimum_required(VERSION 3.10)

# --------------------------------------------------------------
# Adding Option to Enable Sanitizers
# --------------------------------------------------------------

option(ENABLE_SANITIZERS "Enable address/undefined sanitizers" OFF)
# to build sanitized version
# cmake -S . -B build-sanitized -DENABLE_SANITIZERS=ON
# cmake --build build-sanitized

# --------------------------------------------------------------
# Project definition
# --------------------------------------------------------------
project(Magnetic_Simulation LANGUAGES CXX)

if (ENABLE_SANITIZERS)
    if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        add_compile_options(
            -fsanitize=address,undefined
            -fno-omit-frame-pointer
            -g
            -O0
        )

        add_link_options(
            -fsanitize=address,undefined
        )
    else()
        message(WARNING "Sanitizers not supported for this compiler")
    endif()
endif()

# C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Export compile_commands.json (clangd, IDEs)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# --------------------------------------------------------------
# Library target (non-template implementation files only)
# --------------------------------------------------------------

# Collect all implementation files (no main())
file(GLOB SRC_FILES
    ${CMAKE_SOURCE_DIR}/src/*.cpp
)

add_library(magnetic_simulation_lib ${SRC_FILES})

# Public include path (headers + .tpp live here)
target_include_directories(magnetic_simulation_lib
    PUBLIC
        ${CMAKE_SOURCE_DIR}/include
)

# --------------------------------------------------------------
# Executables (each file in apps/ has its own main)
# --------------------------------------------------------------

file(GLOB APP_FILES
    ${CMAKE_SOURCE_DIR}/apps/*.cpp
)

foreach(APP_FILE ${APP_FILES})
    get_filename_component(EXE_NAME ${APP_FILE} NAME_WE)
    set(EXE_NAME "${EXE_NAME}.out")

    add_executable(${EXE_NAME} ${APP_FILE})

    target_link_libraries(${EXE_NAME}
        PRIVATE
            magnetic_simulation_lib
    )

    set_target_properties(${EXE_NAME}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY
                ${CMAKE_SOURCE_DIR}/bin
    )
endforeach()
