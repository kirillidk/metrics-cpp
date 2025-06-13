cmake_minimum_required(VERSION 3.31.4)
project(metrics-lib-cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(
    ${PROJECT_SOURCE_DIR}/include
)

add_library(metrics-lib-cpp
    src/metrics.cpp
    src/registry.cpp
)

file(GLOB EXAMPLE_SRCS
    ${PROJECT_SOURCE_DIR}/examples/*.cpp
)

foreach(example_src ${EXAMPLE_SRCS})
    get_filename_component(example_name ${example_src} NAME_WE)
    add_executable(${example_name} ${example_src})
    target_link_libraries(${example_name} PRIVATE metrics-lib-cpp)
endforeach()