#include <chrono>  // std::chrono
#include <dumper.hpp>
#include <format>  // std::format
#include <registry.hpp>
#include <visitors.hpp>

namespace Metrics {

void Dumper::write(std::shared_ptr<Metrics::Registry> registry) {
    m_os << getCurrentTimestamp();

    StringValueVisitor sv_visitor;
    ResetVisitor reset_visitor;

    for (const auto& [metric_name, metric_value] : registry->getMetricGroup()) {
        sv_visitor.setMetricName(metric_name);
        metric_value->accept(sv_visitor);
        metric_value->accept(reset_visitor);
    }

    m_os << sv_visitor.getResult() << '\n';
    m_os.flush();
}

void Dumper::enableAutoWrite(
    std::shared_ptr<Metrics::Registry> registry, std::chrono::seconds interval
) {
    if (m_worker.joinable()) return;

    auto weak_self = weak_from_this();

    m_worker =
        std::jthread([weak_self, registry, interval](std::stop_token st) {
            while (!st.stop_requested()) {
                if (auto self = weak_self.lock()) {
                    self->write(registry);
                } else {
                    break;
                }
                std::this_thread::sleep_for(interval);
            }
        });
}

void Dumper::disableAutoWrite() { m_worker.request_stop(); }

std::string getCurrentTimestamp() {
    auto const tp = std::chrono::system_clock::now();
    auto const ts = std::chrono::floor<std::chrono::seconds>(tp);
    auto const ms =
        std::chrono::duration_cast<std::chrono::milliseconds>(tp - ts).count();

    auto const lt = std::chrono::current_zone()->to_local(ts);
    return std::format("{:%Y-%m-%d %H:%M:%S}.{:03}", lt, ms);
}

}  // namespace Metrics