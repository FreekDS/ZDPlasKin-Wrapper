cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)

project(ZDPlaskin)

add_subdirectory(fortran)

include_directories(include)

file(COPY ./libs/ DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/libs/")

set(SRC
        ${CMAKE_CURRENT_SOURCE_DIR}/src/WindowsLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/UnixLoader.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ZDPlasKinWrapper.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ZDPlasKinParams.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/helpers/string_helpers.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/src/ZDPlasKinCompiler.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/examples/example1.cpp)

add_executable(LZDPlaskin ${SRC} main.cpp)

add_subdirectory(python)
