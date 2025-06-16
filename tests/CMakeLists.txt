cmake_minimum_required(VERSION 3.31.4)
project(metrics-unit-tests LANGUAGES CXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(unit_tests 
    metrics_test.cpp
    registry_test.cpp
    dumper_test.cpp
)
target_link_libraries(unit_tests
    PRIVATE
        metrics-lib-cpp
        Catch2::Catch2WithMain
)

add_test(NAME unit_tests COMMAND unit_tests)
set_tests_properties(unit_tests PROPERTIES LABELS "unit")