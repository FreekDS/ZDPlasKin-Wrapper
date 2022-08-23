cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)

project(ZDPlaskin)

add_subdirectory(fortran)

include_directories(include)

file(COPY ./libs/ DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/libs/")

set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR})

set(SRC
        ${SRC_DIR}/src/WindowsLoader.cpp
        ${SRC_DIR}/src/UnixLoader.cpp
        ${SRC_DIR}/src/ZDPlasKinWrapper.cpp
        ${SRC_DIR}/src/ZDPlasKinParams.cpp
        ${SRC_DIR}/src/helpers/string_helpers.cpp
        ${SRC_DIR}/src/ZDPlasKinCompiler.cpp
        ${SRC_DIR}/examples/example1.cpp)

add_executable(LZDPlaskin ${SRC} ${SRC_DIR}/main.cpp)


add_subdirectory(python)
