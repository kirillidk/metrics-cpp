find_package(Catch2 REQUIRED)

file(GLOB_RECURSE SOURCES CONFIGURE_DEPENDS
        "${CMAKE_CURRENT_SOURCE_DIR}/*.cpp"
)

add_executable(integration_tests ${SOURCES})
target_link_libraries(integration_tests
    PRIVATE
        ${METRICS_CPP_LIB}
        Catch2::Catch2WithMain
)

add_test(NAME integration_tests COMMAND integration_tests)
set_tests_properties(integration_tests PROPERTIES LABELS "integration")