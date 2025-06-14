#pragma once

#include <dumper.hpp>
#include <memory>  // std::shared_ptr
#include <metrics.hpp>
#include <string>         // std::string
#include <string_view>    // std::string_view
#include <unordered_map>  // std::unordered_map

namespace Metrics {

class Registry {
private:
    std::mutex m_mutex;
    std::unordered_map<std::string, std::shared_ptr<Counter>> m_metrics;
public:
    void addMetric(
        std::string_view metric_name,
        const std::shared_ptr<Counter> metric_value
    );

    template <typename MetricType>
    std::shared_ptr<MetricType> getMetric(std::string_view metric_name) {
        static_assert(
            std::is_same_v<MetricType, Counter>, "Unsupported metric type"
        );

        std::lock_guard<std::mutex> lock(m_mutex);
        std::string key {metric_name};

        if (m_metrics.find(key) == m_metrics.end()) {
            m_metrics[key] = std::make_shared<MetricType>();
        }

        return m_metrics[key];
    }

    std::unordered_map<std::string, std::shared_ptr<Counter>> getMetricGroup();
};

std::shared_ptr<Registry> getRegistry();
std::shared_ptr<Registry> createRegistry();

}  // namespace Metrics