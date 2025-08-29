#include <catch2/catch_test_macros.hpp>
#include <chrono>
#include <dumper.hpp>
#include <filesystem>
#include <fstream>
#include <metrics.hpp>
#include <registry.hpp>
#include <thread>

TEST_CASE("Dumper basic functionality", "[dumper]") {
    const std::string test_filename = "dumper_test.txt";

    // Clean up test file
    if (std::filesystem::exists(test_filename)) {
        std::filesystem::remove(test_filename);
    }

    auto reg = Metrics::createRegistry();
    auto dumper = std::make_shared<Metrics::Dumper>(test_filename);

    SECTION("Manual write to file") {
        reg->addMetric("test_counter", std::make_shared<Metrics::Counter>(42));
        reg->addMetric("test_gauge", std::make_shared<Metrics::Gauge>(3.14));

        dumper->write(reg);

        REQUIRE(std::filesystem::exists(test_filename));

        std::ifstream file(test_filename);
        REQUIRE(file.is_open());

        std::string content(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );
        file.close();

        REQUIRE(!content.empty());
        REQUIRE(content.find("test_counter") != std::string::npos);
        REQUIRE(content.find("test_gauge") != std::string::npos);
        REQUIRE(content.find("42") != std::string::npos);
        REQUIRE(content.find("3.14") != std::string::npos);
    }

    SECTION("Auto write functionality") {
        reg->addMetric("auto_counter", std::make_shared<Metrics::Counter>(1));

        dumper->enableAutoWrite(reg, std::chrono::seconds(1));

        // Let it run for a short time
        std::this_thread::sleep_for(std::chrono::seconds(2));

        dumper->disableAutoWrite();

        REQUIRE(std::filesystem::exists(test_filename));

        std::ifstream file(test_filename);
        std::string content(
            (std::istreambuf_iterator<char>(file)),
            std::istreambuf_iterator<char>()
        );
        file.close();

        REQUIRE(!content.empty());
        REQUIRE(content.find("auto_counter") != std::string::npos);
    }

    SECTION("Reset functionality") {
        dumper->write(reg);
        REQUIRE(std::filesystem::exists(test_filename));

        dumper->reset();
        // File should still exist but dumper should be reset
        REQUIRE(std::filesystem::exists(test_filename));
    }

    // Clean up
    dumper->reset();
    if (std::filesystem::exists(test_filename)) {
        std::filesystem::remove(test_filename);
    }
}