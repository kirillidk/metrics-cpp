add_executable(unit_tests 
    metrics_test.cpp
    registry_test.cpp
    dumper_test.cpp
)
target_link_libraries(unit_tests
    PRIVATE
        metrics-lib-cpp
        Catch2::Catch2WithMain
)

add_test(NAME unit_tests COMMAND unit_tests)
set_tests_properties(unit_tests PROPERTIES LABELS "unit")