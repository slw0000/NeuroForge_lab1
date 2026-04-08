cmake_minimum_required(VERSION 3.10)

project(NeuralNetworkLab1 VERSION 1.0)

set(CMAKE_CXX_STANDARD 20)

include_directories(
        ${CMAKE_SOURCE_DIR}/include
)

add_executable(main src/main.cpp
        src/file_import.cpp
        src/matrix_realization.cpp
        src/visualization.cpp
        src/utils.cpp
        tests/matrix_tests.cpp
        tests/file_Import_tests.cpp
        tests/visualization_tests.cpp
)

file(COPY ${CMAKE_SOURCE_DIR}/data
        DESTINATION ${CMAKE_BINARY_DIR})

file(COPY ${CMAKE_SOURCE_DIR}/scripts/
        DESTINATION ${CMAKE_BINARY_DIR}/scripts)