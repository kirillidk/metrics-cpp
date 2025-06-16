cmake_minimum_required(VERSION 3.20)
project(metric-lib-cpp-example)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)

FetchContent_Declare(
    metrics
    GIT_REPOSITORY https://github.com/kirillidk/metrics-lib-cpp
    GIT_TAG        main
)
FetchContent_MakeAvailable(metrics)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE metrics-lib-cpp)