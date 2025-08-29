find_package(Catch2 REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(unit_tests ${SOURCES})
target_link_libraries(unit_tests
    PRIVATE
        ${METRICS_CPP_LIB}
        Catch2::Catch2WithMain
)

add_test(NAME unit_tests COMMAND unit_tests)
set_tests_properties(unit_tests PROPERTIES LABELS "unit")