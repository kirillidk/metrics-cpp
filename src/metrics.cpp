#include <atomic>  // std::atomic
#include <metrics.hpp>

namespace Metrics {

void ICounter::accept(IMetricsVisitor& visitor) {
    visitor.visit(shared_from_this());
}

class CounterImpl : public ICounter {
private:
    std::atomic<uint64_t> m_value;
public:
    CounterImpl() noexcept { m_value.store(0); }
    CounterImpl(uint64_t value) noexcept { m_value.store(value); }
    CounterImpl(const CounterImpl&) = delete;
    CounterImpl(CounterImpl&&) = delete;

    uint64_t value() const override {
        return m_value.load(std::memory_order_acquire);
    }
    void reset() override { m_value.store(0, std::memory_order_release); }

    ICounter& operator++(int) override {
        m_value.fetch_add(1, std::memory_order_acq_rel);
        return *this;
    }
    ICounter& operator+=(uint64_t value) override {
        m_value.fetch_add(value, std::memory_order_acq_rel);
        return *this;
    }
};

std::shared_ptr<ICounter> createCounter() {
    return std::make_shared<CounterImpl>();
}

}  // namespace Metrics