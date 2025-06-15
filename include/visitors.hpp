#pragma once

#include <metrics.hpp>
#include <sstream>

namespace Metrics {

template <typename MetricType>
class ValueVisitor : public IMetricsVisitor {
public:
    MetricType m_value;
    void visit(std::shared_ptr<ICounter> counter) override {
        m_value = MetricType(counter);
    }
};

class ResetVisitor : public IMetricsVisitor {
public:
    void visit(std::shared_ptr<ICounter> counter) override { counter->reset(); }
};

class StringValueVisitor : public IMetricsVisitor {
private:
    std::ostringstream m_stream;
    std::string m_metric_name;
public:
    void setCurrentName(const std::string& metric_name) {
        m_metric_name = metric_name;
    }

    void visit(std::shared_ptr<ICounter> counter) override {
        m_stream << " \"" << m_metric_name << "\" " << counter->value();
    }

    std::string getResult() const { return m_stream.str(); }
};

}  // namespace Metrics