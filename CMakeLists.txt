cmake_minimum_required(VERSION 3.20)
project(metrics-cpp LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(METRICS_CPP_LIB "${CMAKE_PROJECT_NAME}_lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

option(BUILD_INTEGRATION "Build integration tests" OFF)
option(BUILD_UNIT "Build unit tests" OFF)

enable_testing()

add_subdirectory(src)
add_subdirectory(examples)

if(BUILD_INTEGRATION)
  add_subdirectory(integration)
endif()

if(BUILD_UNIT)
  add_subdirectory(unit)
endif()