cmake_minimum_required(VERSION 3.31.4)
project(metrics-integration-test LANGUAGES CXX)

set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

add_executable(integration_test integration_test.cpp)
target_link_libraries(integration_test
    PRIVATE
        metrics-lib-cpp
        Catch2::Catch2WithMain
)

add_test(NAME integration_test COMMAND integration_test)
set_tests_properties(integration_test PROPERTIES LABELS "integration")