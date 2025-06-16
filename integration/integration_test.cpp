#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <dumper.hpp>
#include <filesystem>
#include <fstream>
#include <metrics.hpp>
#include <registry.hpp>
#include <thread>
#include <visitors.hpp>

TEST_CASE("Complete metrics library integration test", "[integration]") {
    const std::string test_filename = "integration_test.txt";

    // Clean up test file
    if (std::filesystem::exists(test_filename)) {
        std::filesystem::remove(test_filename);
    }

    // Full integration test similar to examples.cpp
    std::shared_ptr<Metrics::Registry> reg = Metrics::getRegistry();
    auto dumper = std::make_shared<Metrics::Dumper>(test_filename);

    // Test shared counter behavior
    {
        Metrics::Counter cnt1 {35};
        Metrics::Counter cnt2 = cnt1;  // shared metric

        cnt1++;
        REQUIRE(cnt1.value() == 36);
        REQUIRE(cnt2.value() == 36);  // both should have same value

        cnt2.reset();
        REQUIRE(cnt1.value() == 0);  // both reset
        REQUIRE(cnt2.value() == 0);
    }

    // Test registry operations with different metric types
    {
        Metrics::Gauge cpuUtilization {0.97};
        reg->addMetric("CPU", cpuUtilization.get_ptr());
        reg->addMetric("HTTP_RPS", std::make_shared<Metrics::Counter>(42));

        Metrics::Counter httpRPS = reg->getMetric<Metrics::Counter>("HTTP_RPS");
        httpRPS += 20;

        REQUIRE(httpRPS.value() == 62);
        REQUIRE(cpuUtilization.value() == Catch::Approx(0.97));
    }

    // Test auto-dumping functionality
    {
        // Add more metrics for comprehensive test
        reg->addMetric("ERRORS", std::make_shared<Metrics::Counter>(5));
        reg->addMetric("MEMORY", std::make_shared<Metrics::Gauge>(78.5));

        // Modify metrics
        Metrics::Counter errors = reg->getMetric<Metrics::Counter>("ERRORS");
        Metrics::Gauge memory = reg->getMetric<Metrics::Gauge>("MEMORY");

        errors += 3;
        memory += 5.2;

        REQUIRE(errors.value() == 8);
        REQUIRE(memory.value() == Catch::Approx(83.7));

        // Enable auto-dumping for short period
        dumper->enableAutoWrite(reg, std::chrono::seconds(1));

        // Let it dump a couple of times
        std::this_thread::sleep_for(std::chrono::seconds(1));

        dumper->disableAutoWrite();
    }

    // Verify file output contains expected data
    REQUIRE(std::filesystem::exists(test_filename));

    std::ifstream file(test_filename);
    REQUIRE(file.is_open());

    std::string content(
        (std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>()
    );
    file.close();

    // Verify all metrics were dumped
    REQUIRE(!content.empty());
    REQUIRE(content.find("CPU") != std::string::npos);
    REQUIRE(content.find("HTTP_RPS") != std::string::npos);
    REQUIRE(content.find("ERRORS") != std::string::npos);
    REQUIRE(content.find("MEMORY") != std::string::npos);

    // Verify timestamp format is present
    REQUIRE(content.find("-") != std::string::npos);  // date separator
    REQUIRE(content.find(":") != std::string::npos);  // time separator

    // Clean up
    std::filesystem::remove(test_filename);
}