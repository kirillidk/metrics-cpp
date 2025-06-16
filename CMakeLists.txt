cmake_minimum_required(VERSION 3.31.4)
project(metrics-lib-cpp)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(METRICS_BUILD_EXAMPLES "Build examples" OFF)

add_library(metrics-lib-cpp
    src/metrics.cpp
    src/registry.cpp
    src/dumper.cpp
)

target_compile_features(metrics-lib-cpp
    PUBLIC
      cxx_std_20
)

target_include_directories(metrics-lib-cpp
    PUBLIC
      $<BUILD_INTERFACE:${PROJECT_SOURCE_DIR}/include>
      $<INSTALL_INTERFACE:include>
)

option(METRICS_BUILD_EXAMPLES "Build examples" ON)
if (METRICS_BUILD_EXAMPLES)
  file(GLOB EXAMPLE_SRCS ${PROJECT_SOURCE_DIR}/examples/*.cpp)
  foreach(example_src ${EXAMPLE_SRCS})
    get_filename_component(example_name ${example_src} NAME_WE)
    add_executable(${example_name} ${example_src})
    target_link_libraries(${example_name} PRIVATE metrics-lib-cpp)
  endforeach()
endif()
