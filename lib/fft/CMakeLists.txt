cmake_minimum_required(VERSION 2.8)
add_library(fft fft/Fft.cpp)
target_include_directories(fft INTERFACE fft)
set(CMAKE_CXX_STANDARD 11)
