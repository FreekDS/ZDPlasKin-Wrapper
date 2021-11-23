include(FetchContent)


FetchContent_Declare(
        pybind11
        GIT_REPOSITORY https://github.com/pybind/pybind11
        GIT_TAG        v2.8.1
        GIT_SHALLOW    TRUE
)

FetchContent_MakeAvailable(pybind11)
