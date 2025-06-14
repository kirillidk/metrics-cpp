#include <metrics.hpp>
#include <registry.hpp>

namespace Metrics {

void Registry::addMetric(
    std::string_view metric_name, const std::shared_ptr<Counter> metric_value
) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_metrics[std::string(metric_name)] = metric_value;
};

std::unordered_map<std::string, std::shared_ptr<Counter>>
Registry::getMetricGroup() {
    return m_metrics;
}

std::shared_ptr<Registry> getRegistry() {
    static std::shared_ptr<Registry> newRegistry = createRegistry();
    return newRegistry;
}

std::shared_ptr<Registry> createRegistry() {
    return std::make_shared<Registry>();
}

}  // namespace Metrics