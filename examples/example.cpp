#include <dumper.hpp>
#include <metrics.hpp>
#include <registry.hpp>

int main() {
    auto reg = Metrics::getRegistry();

    auto counter = std::make_shared<Metrics::Counter>(35);
    reg->addMetric("HTTP requests RPS", counter);

    auto httpRPS = reg->getMetric<Metrics::Counter>("HTTP requests RPS1");

    httpRPS->operator+=(32);
    httpRPS->reset();

    auto dumper = std::make_shared<Metrics::Dumper>("example.txt");
    auto worker = dumper->autoWrite(reg, std::chrono::seconds(1));

    // some work
    std::this_thread::sleep_for(std::chrono::seconds(4));
}