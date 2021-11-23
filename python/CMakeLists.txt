include(pybind11.cmake)
pybind11_add_module(ZDPlaskinderen ${SRC} bindings.cpp)
