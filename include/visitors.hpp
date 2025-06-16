#pragma once

#include <metrics.hpp>
#include <sstream>      // std::ostringstream
#include <type_traits>  // std::is_same_v

namespace Metrics {

template <typename MetricType>
class ValueVisitor : public IMetricsVisitor {
private:
    MetricType m_value;
public:
    void visit(std::shared_ptr<ICounter> counter) override {
        if constexpr (std::is_same_v<MetricType, Counter>) {
            m_value = MetricType(counter);
        }
    }

    void visit(std::shared_ptr<IGauge> gauge) override {
        if constexpr (std::is_same_v<MetricType, Gauge>) {
            m_value = MetricType(gauge);
        }
    }

    MetricType getResult() const { return m_value; }
};

class ResetVisitor : public IMetricsVisitor {
public:
    void visit(std::shared_ptr<ICounter> counter) override { counter->reset(); }
    void visit(std::shared_ptr<IGauge> counter) override { counter->reset(); }
};

class StringValueVisitor : public IMetricsVisitor {
private:
    std::ostringstream m_stream;
    std::string m_metric_name;
public:
    void visit(std::shared_ptr<ICounter> counter) override {
        m_stream << " \"" << m_metric_name << "\" " << counter->value();
    }

    void visit(std::shared_ptr<IGauge> gauge) override {
        m_stream << " \"" << m_metric_name << "\" " << gauge->value();
    }

    void setMetricName(const std::string& metric_name) {
        m_metric_name = metric_name;
    }

    std::string getResult() const { return m_stream.str(); }
};

}  // namespace Metrics