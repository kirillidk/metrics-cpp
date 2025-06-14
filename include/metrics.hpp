#pragma once

#include <atomic>  // std::atomic

namespace Metrics {

class IMetrics {
public:
    virtual ~IMetrics() = default;
    virtual void reset() = 0;
};

class Counter : public IMetrics {
private:
    std::atomic<uint64_t> m_value;
public:
    Counter() noexcept { m_value.store(0); };
    Counter(uint64_t value) noexcept { m_value.store(value); };
    Counter(const Counter&) = delete;
    Counter(Counter&&) = delete;
    ~Counter() = default;

    Counter& operator++(int) {
        m_value.fetch_add(1, std::memory_order_acq_rel);
        return *this;
    };
    Counter& operator+=(uint32_t value) {
        m_value.fetch_add(value, std::memory_order_acq_rel);
        return *this;
    };
    uint64_t value() const { return m_value.load(std::memory_order_acquire); };
    void reset() override { m_value.store(0, std::memory_order_release); };
};

}  // namespace Metrics