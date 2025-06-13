#include <iostream>
#include <metrics.hpp>
#include <registry.hpp>

int main() {
    auto reg = Metrics::getRegistry();

    auto counter = std::make_shared<Metrics::Counter>();
    reg->addMetric("HTTP requests RPS", counter);

    auto httpRPS = reg->getCounter("HTTP requests RPS");

    httpRPS->operator+=(32);
    httpRPS->reset();

    std::cout << httpRPS->value();
}