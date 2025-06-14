#include <dumper.hpp>
#include <metrics.hpp>
#include <registry.hpp>

int main() {
    auto reg = Metrics::getRegistry();

    Metrics::Counter cnt {35};
    cnt.reset();
    cnt++;

    // reg->addMetric("HTTP requests RPS", cnt);

    // auto httpRPS = reg->getMetric<Metrics::Counter>("HTTP requests RPS1");

    // httpRPS+=32;
    // httpRPS->reset();

    // auto dumper = std::make_shared<Metrics::Dumper>("example.txt");
    // auto worker = dumper->autoWrite(reg, std::chrono::seconds(1));

    // // some work
    // std::this_thread::sleep_for(std::chrono::seconds(4));
}