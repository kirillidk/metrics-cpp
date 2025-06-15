#pragma once

#include <atomic>  // std::atomic
#include <memory>  // std::shared_ptr

namespace Metrics {

// forward declarations
class ICounter;

std::shared_ptr<ICounter> createCounter();

class IMetricsVisitor {
public:
    virtual void visit(ICounter&) = 0;
};

class IMetrics {
public:
    virtual ~IMetrics() = default;
    virtual void accept(IMetricsVisitor&) = 0;
};

template <typename T>
class Wrapper : public T {
protected:
    std::shared_ptr<T> m_value;
    Wrapper(std::shared_ptr<T> value) : m_value(value) {}
public:
    std::shared_ptr<T> get_ptr() { return m_value; }
};

class ICounter : public IMetrics {
public:
    virtual uint64_t value() const = 0;
    virtual void reset() = 0;
    virtual ICounter& operator++(int) = 0;
    virtual ICounter& operator+=(uint64_t value) = 0;

    void accept(IMetricsVisitor&) override;
};

class Counter : public Wrapper<ICounter> {
public:
    Counter() noexcept : Wrapper(createCounter()) {}
    Counter(std::shared_ptr<ICounter> value) noexcept : Wrapper(value) {}
    Counter(uint64_t value) noexcept : Wrapper(createCounter()) {
        *m_value += value;
    }
    Counter(const Counter&) = default;
    Counter(Counter&&) = default;
    Counter& operator=(const Counter&) = default;
    Counter& operator=(Counter&&) = default;

    uint64_t value() const override { return m_value->value(); }
    void reset() { m_value->reset(); }
    ICounter& operator++(int) { return (*m_value)++; }
    ICounter& operator+=(uint64_t value) { return (*m_value += value); }
};

}  // namespace Metrics