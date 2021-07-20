cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 14)

project(ZDPlaskin)

add_subdirectory(fortran)

include_directories(include)

add_executable(ZDPlaskin main.cpp src/WindowsLoader.cpp src/UnixLoader.cpp include/unix/UnixLoader.h include/ZDPlasKinFunctionDefs.h)

