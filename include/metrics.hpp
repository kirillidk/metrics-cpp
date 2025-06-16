#pragma once

#include <atomic>  // std::atomic
#include <memory>  // std::shared_ptr

namespace Metrics {

// forward declarations
class ICounter;
class IGauge;

std::shared_ptr<ICounter> createCounter();
std::shared_ptr<IGauge> createGauge();

class IMetricsVisitor {
public:
    virtual void visit(std::shared_ptr<ICounter>) = 0;
    virtual void visit(std::shared_ptr<IGauge>) = 0;
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

class ICounter : public IMetrics,
                 public std::enable_shared_from_this<ICounter> {
public:
    virtual uint64_t value() const = 0;
    virtual void reset() = 0;
    virtual ICounter& operator++(int) = 0;
    virtual ICounter& operator+=(uint64_t value) = 0;

    void accept(IMetricsVisitor&) override;
};

class IGauge : public IMetrics, public std::enable_shared_from_this<IGauge> {
public:
    virtual double value() const = 0;
    virtual void reset() = 0;
    virtual IGauge& operator+=(double value) = 0;
    virtual IGauge& operator-=(double value) = 0;

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

class Gauge : public Wrapper<IGauge> {
public:
    Gauge() noexcept : Wrapper(createGauge()) {}
    Gauge(std::shared_ptr<IGauge> value) noexcept : Wrapper(value) {}
    Gauge(double value) noexcept : Wrapper(createGauge()) { *m_value += value; }
    Gauge(const Gauge&) = default;
    Gauge(Gauge&&) = default;
    Gauge& operator=(const Gauge&) = default;
    Gauge& operator=(Gauge&&) = default;

    double value() const override { return m_value->value(); }
    void reset() { m_value->reset(); }
    IGauge& operator+=(double value) { return (*m_value += value); }
    IGauge& operator-=(double value) { return (*m_value -= value); }
};

}  // namespace Metrics