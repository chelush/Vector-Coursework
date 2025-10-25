cmake_minimum_required(VERSION 3.16)
project(VectorCoursework LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include_directories(include)

add_executable(run_tests tests/test_vector.cpp
        include/vector.h)
