#pragma once

#include <string>
#include <vector>

namespace interop {
namespace utils {
std::vector<std::string> split_rx(const std::string & string, const std::string & delimiter);
}
} // namespace interop
