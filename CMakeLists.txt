cmake_minimum_required(VERSION 3.10)

# Set your project name and version
project(FluidSim VERSION 1.0)

# Using gcc as compiler
set(CMAKE_C_COMPILER gcc)
set(CMAKE_CXX_COMPILER g++)

# generates compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Require C++11 or newer
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find raylib package
find_package(raylib REQUIRED)

# Automatically include all .cpp files in the src directory
file(GLOB SOURCES "src/*.cpp")
add_executable(${PROJECT_NAME} ${SOURCES})

# Link raylib to the project
target_link_libraries(${PROJECT_NAME} PRIVATE raylib)
