cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 14)

project(ZDPlaskin)

add_subdirectory(fortran)

include_directories(include)

set(SRC
        main.cpp
        src/WindowsLoader.cpp
        src/UnixLoader.cpp
        src/ZDPlasKinWrapper.cpp
        src/PlaskinParams.cpp src/helpers/string_helpers.cpp)

add_executable(ZDPlaskin ${SRC})

