find_package(Python3 COMPONENTS Development.Module)

if(PYTHON3_FOUND)
    message(STATUS "python found :)")
endif()

add_library(py_module_test SHARED ${SRC} zdplaskin_bindings.cpp)
target_include_directories(py_module_test PUBLIC ${Python3_INCLUDE_DIRS})

set_target_properties(py_module_test
        PROPERTIES
        PREFIX ""
        OUTPUT_NAME "test_module"
        LINKER_LANGUAGE CXX
        SUFFIX ".pyd")

target_link_libraries(py_module_test ${Python3_LIBRARIES})