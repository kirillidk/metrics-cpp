#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <metrics.hpp>
#include <registry.hpp>

TEST_CASE("Registry basic functionality", "[registry]") {
    auto reg =
        Metrics::createRegistry();  // Create separate registry for testing

    SECTION("Add and retrieve metrics") {
        auto counter = std::make_shared<Metrics::Counter>(10);
        auto gauge = std::make_shared<Metrics::Gauge>(3.14);

        reg->addMetric("test_counter", counter->get_ptr());
        reg->addMetric("test_gauge", gauge->get_ptr());

        auto retrieved_counter =
            reg->getMetric<Metrics::Counter>("test_counter");
        auto retrieved_gauge = reg->getMetric<Metrics::Gauge>("test_gauge");

        REQUIRE(retrieved_counter.value() == 10);
        REQUIRE(retrieved_gauge.value() == Catch::Approx(3.14));
    }

    SECTION("Get non-existent metric creates default") {
        auto new_counter = reg->getMetric<Metrics::Counter>("new_counter");
        auto new_gauge = reg->getMetric<Metrics::Gauge>("new_gauge");

        REQUIRE(new_counter.value() == 0);
        REQUIRE(new_gauge.value() == Catch::Approx(0.0));
    }

    SECTION("Metric modifications persist") {
        reg->addMetric(
            "persistent_counter", std::make_shared<Metrics::Counter>(5)
        );

        auto counter1 = reg->getMetric<Metrics::Counter>("persistent_counter");
        counter1 += 10;

        auto counter2 = reg->getMetric<Metrics::Counter>("persistent_counter");
        REQUIRE(counter2.value() == 15);  // Should persist the change
    }

    SECTION("Get metric group") {
        reg->addMetric("metric1", std::make_shared<Metrics::Counter>(1));
        reg->addMetric("metric2", std::make_shared<Metrics::Gauge>(2.0));

        auto metrics = reg->getMetricGroup();
        REQUIRE(metrics.size() >= 2);
        REQUIRE(metrics.find("metric1") != metrics.end());
        REQUIRE(metrics.find("metric2") != metrics.end());
    }
}