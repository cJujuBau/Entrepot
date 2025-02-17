cmake_minimum_required(VERSION 3.10)

# Set the project name
project(RobotController)

# Specify the C++ standard
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Include directories
include_directories(include)

# Source files
set(SOURCES
    src/main.cpp
    src/MotorController.cpp
    src/utils/Utils.cpp
)

# Create the executable
add_executable(RobotController ${SOURCES})

target_link_libraries(robot-controller wiringPi)