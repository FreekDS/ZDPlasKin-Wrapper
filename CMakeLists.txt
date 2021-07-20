cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 14)

project(ZDPlaskin)

add_subdirectory(fortran)


add_executable(ZDPlaskin main.cpp)

