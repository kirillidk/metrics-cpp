#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>
#include <metrics.hpp>

TEST_CASE("Counter basic functionality", "[counter]") {
    SECTION("Default initialization") {
        Metrics::Counter counter;
        REQUIRE(counter.value() == 0);
    }

    SECTION("Value initialization") {
        Metrics::Counter counter(42);
        REQUIRE(counter.value() == 42);
    }

    SECTION("Increment operator") {
        Metrics::Counter counter(10);
        counter++;
        REQUIRE(counter.value() == 11);
    }

    SECTION("Addition operator") {
        Metrics::Counter counter(5);
        counter += 15;
        REQUIRE(counter.value() == 20);
    }

    SECTION("Reset functionality") {
        Metrics::Counter counter(100);
        counter.reset();
        REQUIRE(counter.value() == 0);
    }

    SECTION("Shared counter behavior") {
        Metrics::Counter counter1(25);
        Metrics::Counter counter2 = counter1;  // shared metric

        counter1++;
        REQUIRE(counter1.value() == 26);
        REQUIRE(counter2.value() == 26);  // both should have same value

        counter2 += 10;
        REQUIRE(counter1.value() == 36);
        REQUIRE(counter2.value() == 36);

        counter1.reset();
        REQUIRE(counter1.value() == 0);  // both reset
        REQUIRE(counter2.value() == 0);
    }
}

TEST_CASE("Gauge basic functionality", "[gauge]") {
    SECTION("Default initialization") {
        Metrics::Gauge gauge;
        REQUIRE(gauge.value() == Catch::Approx(0.0));
    }

    SECTION("Value initialization") {
        Metrics::Gauge gauge(3.14);
        REQUIRE(gauge.value() == Catch::Approx(3.14));
    }

    SECTION("Addition operator") {
        Metrics::Gauge gauge(10.5);
        gauge += 5.25;
        REQUIRE(gauge.value() == Catch::Approx(15.75));
    }

    SECTION("Subtraction operator") {
        Metrics::Gauge gauge(20.0);
        gauge -= 7.5;
        REQUIRE(gauge.value() == Catch::Approx(12.5));
    }

    SECTION("Reset functionality") {
        Metrics::Gauge gauge(99.99);
        gauge.reset();
        REQUIRE(gauge.value() == Catch::Approx(0.0));
    }

    SECTION("Shared gauge behavior") {
        Metrics::Gauge gauge1(42.0);
        Metrics::Gauge gauge2 = gauge1;  // shared metric

        gauge1 += 8.0;
        REQUIRE(gauge1.value() == Catch::Approx(50.0));
        REQUIRE(
            gauge2.value() == Catch::Approx(50.0)
        );  // both should have same value

        gauge2 -= 15.0;
        REQUIRE(gauge1.value() == Catch::Approx(35.0));
        REQUIRE(gauge2.value() == Catch::Approx(35.0));

        gauge1.reset();
        REQUIRE(gauge1.value() == Catch::Approx(0.0));  // both reset
        REQUIRE(gauge2.value() == Catch::Approx(0.0));
    }
}