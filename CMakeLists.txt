cmake_minimum_required(VERSION 4.0)
project(TVector)

set(CMAKE_CXX_STANDARD 14)

add_library(TVector STATIC TVector.cpp)

add_executable(Tests Tests.cpp)

target_link_libraries(Tests TVector)