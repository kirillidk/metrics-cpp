#include <iostream>  // std::cout
#include <memory>    // std::shared_ptr

#include <metrics.hpp>
#include <registry.hpp>
#include <dumper.hpp>

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
        Metrics::Gauge cpuUtilization {0.97};

        reg->addMetric("CPU", cpuUtilization.get_ptr());
        reg->addMetric("HTTP RPS", std::make_shared<Metrics::Counter>(42));

        Metrics::Counter httpRPS = reg->getMetric<Metrics::Counter>("HTTP RPS");
        httpRPS += 20;

        std::cout << httpRPS.value() << '\n';  // 62
    }

    {
        dumper->enableAutoWrite(
            reg, std::chrono::seconds(1)
        );  // asynchronously writes for the lifetime of Dumper object
    }

    // further work in main thread
    std::this_thread::sleep_for(std::chrono::seconds(4));
}