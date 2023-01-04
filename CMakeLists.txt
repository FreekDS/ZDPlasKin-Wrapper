cmake_minimum_required(VERSION 3.19)
set(CMAKE_CXX_STANDARD 20)

project(ZDPlaskin)

include_directories(include)

file(COPY ./libs/ DESTINATION "${CMAKE_CURRENT_BINARY_DIR}/libs/")

set(SRC
        main.cpp
        src/WindowsLoader.cpp
        src/UnixLoader.cpp
        src/ZDPlasKinWrapper.cpp
        src/ZDPlasKinParams.cpp
        src/helpers/string_helpers.cpp
        src/ZDPlasKinCompiler.cpp
        examples/example1.cpp)

add_executable(LZDPlaskin ${SRC})

