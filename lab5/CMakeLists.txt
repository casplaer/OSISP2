cmake_minimum_required(VERSION 3.27)
project(lab5)

set(CMAKE_CXX_STANDARD 23)

# Enable OpenMP support
find_package(OpenMP REQUIRED)
if (OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif ()

add_executable(lab5 main.cpp)
