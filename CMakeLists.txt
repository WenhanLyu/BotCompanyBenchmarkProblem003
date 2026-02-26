cmake_minimum_required(VERSION 3.10)
project(ICPC_Management_System)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Try to use g++-13 if available, otherwise fallback to g++
find_program(GCC13 g++-13)
if(GCC13)
    set(CMAKE_CXX_COMPILER g++-13)
    set(CMAKE_C_COMPILER gcc-13)
    message(STATUS "Using g++-13 compiler")
else()
    message(STATUS "g++-13 not found, using default g++")
endif()

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
set(CMAKE_CXX_FLAGS_DEBUG "-g -O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")

# Set default build type to Release if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

# Source files
set(SOURCES
    main.cpp
)

# Create executable named 'code' in project root
add_executable(code ${SOURCES})

# Set output directory to project root
set_target_properties(code PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_SOURCE_DIR}
)

# Print build configuration
message(STATUS "Build type: ${CMAKE_BUILD_TYPE}")
message(STATUS "C++ compiler: ${CMAKE_CXX_COMPILER}")
message(STATUS "C++ standard: ${CMAKE_CXX_STANDARD}")
