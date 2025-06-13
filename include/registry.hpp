#pragma once

#include <memory>
#include <ostream>
#include <string>
#include <unordered_map>

namespace Metrics {

class Registry {
private:
    std::mutex m_mutex;
    std::unordered_map<std::string, std::shared_ptr<Counter>> m_mp;
public:
    void addMetric(
        const std::string& metric_name,
        const std::shared_ptr<Counter> metric_value
    );
    std::shared_ptr<Counter> getCounter(const std::string& metric_name);
};

std::shared_ptr<Registry> getRegistry();
std::shared_ptr<Registry> createRegistry();

}  // namespace Metrics