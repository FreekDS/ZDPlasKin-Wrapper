#include <pybind11/pybind11.h>
#include <ZDPlasKinWrapper.h>
#include <ZDPlasKinCompiler.h>
#include <platform.h>
#include <iostream>
#include <filesystem>
#include "helpers/string_helpers.h"

#ifdef WINDOWS

#include <windows/WindowsLoader.h>

using PlatformLoader = WindowsLoader;

#endif

#ifdef UNIX

#include "unix/UnixLoader.h"
using PlatformLoader = UnixLoader;

#endif

static PlatformLoader libraryLoader{};
static ZDPlasKinParams params{};
static ZDPlasKinWrapper zdp{};

namespace py = pybind11;


void preprocess(const std::string &kinetFile) {
    ZDPlasKinCompiler::preprocess(kinetFile);
    ZDPlasKinCompiler::compile();

    std::string srcPath = std::filesystem::canonical(kinetFile).string();
    utils::removeSubstr(srcPath, kinetFile);
    std::string path = srcPath + "zdplaskin.dll";

    libraryLoader = PlatformLoader{path};
    params = ZDPlasKinParams{srcPath + "zdplaskin_m.F90"};

    zdp.setLibrary(&libraryLoader);
    zdp.setParams(&params);

    loader.init();
    parameters.readParams()
}


PYBIND11_MODULE(ZDPlaskin, m) {

    m.doc() = R"pbdoc(
        Pybind11 example plugin
        -----------------------
        .. currentmodule:: scikit_build_example
        .. autosummary::
           :toctree: _generate
           add
           subtract
    )pbdoc";

    m.def("test", []() { std::cout << "Test from CPP" << std::endl; }, R"pbdoc(
        Test the CPP bindings
    )pbdoc");

    m.def("preprocess", &preprocess, R"pbdoc(
        Preprocess the kinet input file in order to generate the Fortran Library
        -- kinetFile: absolute path to the input file
    )pbdoc");

}

