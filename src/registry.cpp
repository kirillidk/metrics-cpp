#include <chrono>
#include <metrics.hpp>
#include <mutex>
#include <registry.hpp>

namespace Metrics {

void Registry::addMetric(
    const std::string& metric_name, const std::shared_ptr<Counter> metric_value
) {
    std::lock_guard<std::mutex> lock(m_mutex);
    m_mp[metric_name] = metric_value;
};

std::shared_ptr<Counter> Registry::getCounter(const std::string& metric_name) {
    std::lock_guard<std::mutex> lock(m_mutex);

    if (m_mp.find(metric_name) == m_mp.end()) {
        m_mp[metric_name] = std::make_shared<Counter>();
    }

    return m_mp[metric_name];
}

std::shared_ptr<Registry> getRegistry() {
    static std::shared_ptr<Registry> newRegistry = createRegistry();
    return newRegistry;
}

std::shared_ptr<Registry> createRegistry() {
    return std::make_shared<Registry>();
}

}