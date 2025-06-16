# metrics-lib-cpp

A high-efficient and thread-safe C++ metrics library

## Features

- **Thread-safe**: Safe concurrent access from multiple threads with atomic operations
- **Periodic file dumps**: Automatic metric collection and persistence to text files
- **Extensible architecture**: Easy to add new metric types through visitor pattern
- **Simple API**: Intuitive interface for creating, updating, and managing metrics

## Usage

### Basic Example

```cpp
#include <dumper.hpp>
#include <iostream>
#include <metrics.hpp>
#include <registry.hpp>

int main() {
    // Get global metrics registry
    std::shared_ptr<Metrics::Registry> reg = Metrics::getRegistry();
    
    // Create file dumper for metrics
    auto dumper = std::make_shared<Metrics::Dumper>("metrics.txt");

    // Counter for discrete events
    Metrics::Counter httpRPS{0};
    httpRPS += 42;
    std::cout << "HTTP RPS: " << httpRPS.value() << '\n'; // 42

    // Gauge for continuous measurements
    Metrics::Gauge cpuUtilization{0.75};
    cpuUtilization += 0.15;
    std::cout << "CPU utilization: " << cpuUtilization.value() << '\n'; // 0.9

    return 0;
}
```

### Shared Ownership

```cpp
{
    Metrics::Counter cnt1{35};
    Metrics::Counter cnt2 = cnt1;  // Two objects share the same metric

    cnt1++;
    std::cout << cnt1.value() << ' ' << cnt2.value() << '\n';  // 36 36

    cnt2.reset();
    std::cout << cnt1.value() << ' ' << cnt2.value() << '\n';  // 0 0
}
```

### Working with Registry

```cpp
{
    std::shared_ptr<Metrics::Registry> reg = Metrics::getRegistry();
    
    // Add counter metric to registry
    Metrics::Counter httpCounter{345};
    reg->addMetric("HTTP RPS", httpCounter.get_ptr());

    // Add gauge metric to registry
    Metrics::Gauge cpuUtilization{0.97};
    reg->addMetric("CPU", cpuUtilization.get_ptr());

    // Or create metrics directly
    reg->addMetric("HTTP RPS", std::make_shared<Metrics::Counter>(42));

    // Retrieve metrics from registry
    Metrics::Counter httpRPS = reg->getMetric<Metrics::Counter>("HTTP RPS");
    httpRPS += 20;

    std::cout << httpRPS.value() << '\n';  // 62
}
```

### Automatic File Writing

```cpp
{
    auto dumper = std::make_shared<Metrics::Dumper>("example.txt");
    
    // Enable automatic writing every second
    dumper->enableAutoWrite(reg, std::chrono::seconds(1));
    
    // Metrics will be automatically written to file
    // and reset after each write operation
    
    // Work with metrics...
    std::this_thread::sleep_for(std::chrono::seconds(4));
    
    // Auto-write will stop when dumper is destroyed
}
```

## Output Format

Each metric record is written as a separate line:
```
YYYY-MM-DD HH:MM:SS.mmm "metric_name_1" value1 "metric_name_2" value2 ...
```

Example:
```
2025-06-01 15:00:01.653 "CPU" 0.97 "HTTP RPS" 42
2025-06-01 15:00:02.653 "CPU" 1.12 "HTTP RPS" 30
```

## Build Requirements

- **C++20** or newer (for `std::jthread`, `std::format`)
- **Compiler**: GCC 10+, Clang 12+, MSVC 2022+

### Example CMakeLists.txt

```
cmake_minimum_required(VERSION 3.31.4)
project(metric-lib-cpp-example)

set (CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

include(FetchContent)

FetchContent_Declare(
    metrics
    GIT_REPOSITORY https://github.com/kirillidk/metrics-lib-cpp
    GIT_TAG        main
)
FetchContent_MakeAvailable(metrics)

add_executable(example example.cpp)
target_link_libraries(example PRIVATE metrics-lib-cpp)
```

## Architecture

### Core Components

1. **Metrics** - Base interfaces and metric implementations (Counter, Gauge)
2. **Registry** - Central registry for metric management
3. **Dumper** - Component for writing metrics to files
4. **Visitors** - Visitor pattern for processing different metric types

### Class Diagram

```
IMetrics (interface)
├── ICounter (interface)
│   ├── CounterImpl (atomic implementation)
│   └── Counter (wrapper with shared ownership)
├── IGauge (interface)
│   ├── GaugeImpl (atomic implementation)
│   └── Gauge (wrapper with shared ownership)
│
Registry (thread-safe storage)
├── addMetric()
├── getMetric<T>()
└── getMetricGroup()

Dumper (file output)
├── write()
├── enableAutoWrite()
└── disableAutoWrite()

IMetricsVisitor (interface)
├── ValueVisitor<T>
├── ResetVisitor
└── StringValueVisitor
```

## Thread Safety

- **Registry**: Uses `std::mutex` to protect against race conditions when adding/retrieving metrics
- **CounterImpl**: Uses `std::atomic<uint64_t>` for thread-safe increment operations
- **GaugeImpl**: Uses `std::atomic<double>` with compare-and-swap for thread-safe floating-point operations
- **Dumper**: Automatic writing is performed in a separate thread using `std::jthread`

## Use Cases

### Web Server Monitoring

```cpp
auto reg = Metrics::getRegistry();
auto dumper = std::make_shared<Metrics::Dumper>("server_metrics.txt");

// HTTP request counters
reg->addMetric("HTTP_2XX", std::make_shared<Metrics::Counter>());
reg->addMetric("HTTP_4XX", std::make_shared<Metrics::Counter>());
reg->addMetric("HTTP_5XX", std::make_shared<Metrics::Counter>());

// Performance gauges
reg->addMetric("CPU_Usage", std::make_shared<Metrics::Gauge>());
reg->addMetric("Memory_Usage", std::make_shared<Metrics::Gauge>());
reg->addMetric("Response_Time", std::make_shared<Metrics::Gauge>());

// Auto-write every 5 seconds
dumper->enableAutoWrite(reg, std::chrono::seconds(5));

// In request handlers:
// reg->getMetric<Metrics::Counter>("HTTP_2XX")++;
// reg->getMetric<Metrics::Gauge>("Response_Time") += response_time_ms;
```

### System Resource Monitoring

```cpp
// Simulate CPU and memory metrics with gauges
auto cpuGauge = std::make_shared<Metrics::Gauge>();
auto memoryGauge = std::make_shared<Metrics::Gauge>();

reg->addMetric("CPU_Utilization", cpuGauge);
reg->addMetric("Memory_MB", memoryGauge);

// Example usage:
// cpuGauge += 0.25;  // Increase CPU utilization by 25%
// memoryGauge += 512.0;  // Increase memory usage by 512MB
```

### Performance Monitoring

```cpp
// Track application performance metrics
reg->addMetric("DB_Queries", std::make_shared<Metrics::Counter>());
reg->addMetric("Cache_Hits", std::make_shared<Metrics::Counter>());
reg->addMetric("Cache_Misses", std::make_shared<Metrics::Counter>());

// Track response times and throughput
reg->addMetric("Avg_Response_Time", std::make_shared<Metrics::Gauge>());
reg->addMetric("Request_Rate", std::make_shared<Metrics::Gauge>());

// In application code:
auto dbQueries = reg->getMetric<Metrics::Counter>("DB_Queries");
dbQueries++;  // Increment on each database query

auto responseTime = reg->getMetric<Metrics::Gauge>("Avg_Response_Time");
responseTime += measured_time_ms;  // Add to running average
```

## API Reference

### Core Classes

#### `Metrics::Counter`
- Thread-safe counter with atomic operations
- Supports increment (`++`), addition (`+=`), and reset operations
- Uses `uint64_t` for integer values

#### `Metrics::Gauge`
- Thread-safe gauge for floating-point measurements
- Supports addition (`+=`), subtraction (`-=`), and reset operations
- Uses `double` for floating-point values with atomic compare-and-swap
- Ideal for continuous measurements like CPU usage, temperature, etc.

#### `Metrics::Registry`
- Central storage for all metrics
- Thread-safe add/retrieve operations
- Template-based metric retrieval supporting both Counter and Gauge types

#### `Metrics::Dumper`
- File output management
- Automatic periodic writing with configurable intervals
- Thread-safe operations with proper cleanup
