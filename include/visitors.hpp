#pragma once

#include <metrics.hpp>

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
    void visit(std::shared_ptr<ICounter> counter) override { counter.reset(); }
};

}  // namespace Metrics