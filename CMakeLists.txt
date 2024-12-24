cmake_minimum_required(VERSION 3.10)

# Set your project name and version
project(FluidSim VERSION 1.0)

# Using gcc as compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# Require C++20 or newer
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find raylib package
find_package(raylib REQUIRED)

# Automatically include all .cpp files in the src directory
file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Link raylib to the project
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

# Extra flags for fun
target_compile_options(${PROJECT_NAME} PRIVATE -Wall -Werror -Wsign-conversion  -pedantic)
