include(pybind11.cmake)
pybind11_add_module(ZDPlaskin ${SRC} bindings.cpp)

target_compile_definitions(ZDPlaskin PRIVATE VERSION_INFO=${PROJECT_VERSION})
install(TARGETS ZDPlaskin LIBRARY DESTINATION .)
