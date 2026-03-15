cmake_minimum_required(VERSION 3.10)

project(NeuralNetworkLab1 VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

add_executable(main code/main.cpp
        code/file_import.cpp
        code/file_import.h
        code/matrix_realization.cpp
        code/matrix_realization.h
        code/visualization.cpp
        code/visualization.h
        code/utils.cpp
        code/utils.h
        tests/matrix_tests.cpp
        tests/file_Import_tests.cpp
        tests/visualization_tests.cpp
)

file(COPY ${CMAKE_SOURCE_DIR}/data
        DESTINATION ${CMAKE_BINARY_DIR})

file(COPY ${CMAKE_SOURCE_DIR}/code/plot.py
        DESTINATION ${CMAKE_BINARY_DIR}/code)