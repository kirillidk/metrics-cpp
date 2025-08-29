cmake_minimum_required(VERSION 3.20)
project(metrics-cpp-example)

include(FetchContent)
FetchContent_Declare(
    metrics-cpp
    GIT_REPOSITORY https://github.com/kirillidk/metrics-cpp
    GIT_TAG        main
)
FetchContent_MakeAvailable(metrics-cpp)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE ${METRICS_CPP_LIB})