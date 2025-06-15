#include <dumper.hpp>
#include <iostream>
#include <metrics.hpp>
#include <registry.hpp>
#include <visitors.hpp>

int main() {
    std::shared_ptr<Metrics::Registry> reg = Metrics::getRegistry();

    Metrics::Counter cnt1 {35};
    Metrics::Counter cnt2 = cnt1;  // 2 objects are now share the same metric
    cnt1++;

    std::cout << cnt1.value() << ' ' << cnt2.value() << '\n';  // 36 36
    cnt1.reset();
    std::cout << cnt1.value() << ' ' << cnt2.value() << '\n';  // 0 0

    reg->addMetric("HTTP requests RPS", cnt2.get_ptr());
    reg->addMetric(
        "HTTP requests RPS2", std::make_shared<Metrics::Counter>(87)
    );

    Metrics::Counter httpRPS =
        reg->getMetric<Metrics::Counter>("HTTP requests RPS");
    httpRPS++;

    std::cout << cnt2.value() << ' ' << httpRPS.value() << '\n';  // 1 1

    auto dumper = std::make_shared<Metrics::Dumper>("example.txt");
    std::jthread worker = dumper->autoWrite(reg, std::chrono::seconds(1));

    // some work
    std::this_thread::sleep_for(std::chrono::seconds(4));
}