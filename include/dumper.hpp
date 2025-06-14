#pragma once

#include <fstream>      // std::ofstream
#include <memory>       // std::shared_ptr
#include <string_view>  // std::string_view
#include <thread>       // std::jthread

namespace Metrics {

// forward declaration
class Registry;

class Dumper : public std::enable_shared_from_this<Dumper> {
private:
    std::ofstream m_os;
    const std::string_view m_filename;
public:
    Dumper(std::string_view filename) : m_filename(filename) {
        m_os.open(std::string(filename));
    }

    ~Dumper() {
        if (m_os.is_open()) m_os.close();
    }

    void write(std::shared_ptr<Metrics::Registry> registry);
    std::jthread autoWrite(
        std::shared_ptr<Metrics::Registry> registry,
        std::chrono::seconds interval
    );
};

std::string getCurrentTimestamp();

}  // namespace Metrics