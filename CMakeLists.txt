cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 14)

project(ZDPlaskin)

add_subdirectory(fortran)

include_directories(include)

file(COPY ./libs/ DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/libs/")

set(SRC
        main.cpp
        src/WindowsLoader.cpp
        src/UnixLoader.cpp
        src/ZDPlasKinWrapper.cpp
        src/PlaskinParams.cpp src/helpers/string_helpers.cpp src/ZDPlasKinCompiler.cpp include/ZDPlasKinCompiler.h)

add_executable(LZDPlaskin ${SRC})

