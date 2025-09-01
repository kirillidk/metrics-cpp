file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/*.hpp"
)

add_library(${METRICS_CPP_LIB} STATIC ${SOURCES})

target_include_directories(${METRICS_CPP_LIB} PUBLIC 
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>"
)

target_compile_options(${METRICS_CPP_LIB} PUBLIC
    $<$<CXX_COMPILER_ID:MSVC>:/WX /W4>
    
    $<$<CXX_COMPILER_ID:GNU>:-Werror -Wall -Wextra -Wpedantic -Wno-error=maybe-uninitialized>

    $<$<CXX_COMPILER_ID:Clang>:-Werror -Wall -Wextra -Wpedantic>
)