cmake_minimum_required(VERSION 3.10)

# Use gcc-13 and g++-13 if available (required for OJ)
# On systems without gcc-13, will fall back to default compiler
if(NOT DEFINED CMAKE_C_COMPILER)
    find_program(GCC13 gcc-13)
    if(GCC13)
        set(CMAKE_C_COMPILER gcc-13)
    endif()
endif()
if(NOT DEFINED CMAKE_CXX_COMPILER)
    find_program(GXX13 g++-13)
    if(GXX13)
        set(CMAKE_CXX_COMPILER g++-13)
    endif()
endif()

project(ICPCManagementSystem)

# Set C++ standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Compiler flags
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -O2")

# Source files
file(GLOB SOURCES "*.cpp")

# Create executable named 'code'
add_executable(code ${SOURCES})
