add_executable(integration_test integration_test.cpp)
target_link_libraries(integration_test
    PRIVATE
        metrics-lib-cpp
        Catch2::Catch2WithMain
)

add_test(NAME integration_test COMMAND integration_test)
set_tests_properties(integration_test PROPERTIES LABELS "integration")