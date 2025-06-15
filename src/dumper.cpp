#include <chrono>  // std::chrono
#include <dumper.hpp>
#include <format>  // std::format
#include <registry.hpp>
#include <visitors.hpp>

namespace Metrics {

void Dumper::write(std::shared_ptr<Metrics::Registry> registry) {
    m_os << getCurrentTimestamp();

    // todo
}

std::jthread Dumper::autoWrite(
    std::shared_ptr<Metrics::Registry> registry, std::chrono::seconds interval
) {
    auto wp = weak_from_this();

    return std::jthread([wp, registry, interval](std::stop_token st) {
        while (!st.stop_requested()) {
            if (auto self = wp.lock()) {
                self->write(registry);
            } else {
                break;
            }
            std::this_thread::sleep_for(interval);
        }
    });
}

std::string getCurrentTimestamp() {
    auto const tp = std::chrono::system_clock::now();
    auto const ts = std::chrono::floor<std::chrono::seconds>(tp);
    auto const ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(tp - ts).count();

    auto const lt = std::chrono::current_zone()->to_local(ts);
    return std::format("{:%Y-%m-%d %H:%M:%S}.{:03}", lt, ms);
}

}  // namespace Metrics