cmake_minimum_required(VERSION 3.15)
project(nnlab_python LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(FetchContent)
FetchContent_Declare(pybind11 URL https://github.com/pybind/pybind11/archive/refs/tags/v2.13.6.zip)
FetchContent_MakeAvailable(pybind11)

# Ваши исходники
add_library(nnlab_core STATIC
        src/matrix_realization.cpp
        src/layer.cpp
        src/neural_network.cpp
        src/utils.cpp
)
target_include_directories(nnlab_core PUBLIC include)

# Python-модуль
pybind11_add_module(nnlab_py src/bindings.cpp)
target_link_libraries(nnlab_py PRIVATE nnlab_core pybind11::module)

# Вывод .so/.pyd в папку dist/
set_target_properties(nnlab_py PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/dist"
        RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/dist"
)