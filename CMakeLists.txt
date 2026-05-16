cmake_minimum_required(VERSION 3.10)

# Project Name
project(TrainRouteEngine)

# Specify C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Tell the compiler where to find our header files
include_directories(include)

# Create the executable from our source files
add_executable(TrainEngine src/main.cpp src/Graph.cpp)