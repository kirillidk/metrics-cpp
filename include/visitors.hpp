#pragma once

#include <metrics.hpp>
#include <sstream>

namespace Metrics {

template <typename MetricType>
class ValueVisitor : public IMetricsVisitor {
public:
    MetricType m_value;
    void visit(ICounter& counter) override {
        m_value = MetricType(counter.value());
    }
};

class ResetVisitor : public IMetricsVisitor {
public:
    void visit(ICounter& counter) override { counter.reset(); }
};

}  // namespace Metrics