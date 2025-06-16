cmake_minimum_required(VERSION 3.20)
project(metrics-lib-cpp)
enable_testing()

option(METRICS_BUILD_EXAMPLES "Build examples" OFF)
option(METRICS_BUILD_INTEGRATION "Build integration test" OFF)
option(METRICS_BUILD_TESTS "Build unit tests" OFF)

if(METRICS_BUILD_INTEGRATION OR METRICS_BUILD_TESTS)
  include(FetchContent)
  
  FetchContent_Declare(
      catch2
      GIT_REPOSITORY https://github.com/catchorg/Catch2.git
      GIT_TAG        v3.4.0
  )
  FetchContent_MakeAvailable(catch2)
endif()

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

if (METRICS_BUILD_EXAMPLES)
  file(GLOB EXAMPLE_SRCS ${PROJECT_SOURCE_DIR}/examples/*.cpp)
  foreach(example_src ${EXAMPLE_SRCS})
    get_filename_component(example_name ${example_src} NAME_WE)
    add_executable(${example_name} ${example_src})
    target_link_libraries(${example_name} PRIVATE metrics-lib-cpp)
  endforeach()
endif()

if(METRICS_BUILD_INTEGRATION)
  add_subdirectory(integration)
endif()

if(METRICS_BUILD_TESTS)
  add_subdirectory(tests)
endif()