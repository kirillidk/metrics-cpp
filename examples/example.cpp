#include <dumper.hpp>
#include <iostream>
#include <metrics.hpp>
#include <registry.hpp>
#include <visitors.hpp>

int main() {
    std::shared_ptr<Metrics::Registry> reg = Metrics::getRegistry();
    auto dumper = std::make_shared<Metrics::Dumper>("example.txt");

    {
        Metrics::Counter cnt1 {35};
        Metrics::Counter cnt2 =
            cnt1;  // 2 objects are now share the same metric

        cnt1++;
        std::cout << cnt1.value() << ' ' << cnt2.value() << '\n';  // 36 36

        cnt2.reset();
        std::cout << cnt1.value() << ' ' << cnt2.value() << '\n';  // 0 0
    }

    {
        Metrics::Counter cnt1 {345};
        reg->addMetric("HTTP requests RPS", cnt1.get_ptr());

        reg->addMetric(
            "HTTP requests RPS2", std::make_shared<Metrics::Counter>(1024)
        );

        Metrics::Counter httpRPS =
            reg->getMetric<Metrics::Counter>("HTTP requests RPS");

        httpRPS += 20;

        std::cout << httpRPS.value() << '\n';  // 365
    }

    {
        dumper->enableAutoWrite(
            reg, std::chrono::seconds(1)
        );  // writes for the lifetime of Dumper object
    }

    // some work
    std::this_thread::sleep_for(std::chrono::seconds(4));
}