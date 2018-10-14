#pragma once

#include <string>
#include <unordered_map>
#include <vector>

#include <boost/filesystem/path.hpp>

namespace interop {
struct platform_module_configuration_t {
    std::string name;
    std::vector<boost::filesystem::path> paths;
};

struct platform_configuration_t {
    boost::filesystem::path execution_path;
    std::vector<platform_module_configuration_t> module_configurations;
};

struct native_module_configuration_t {
    std::string name;
    boost::filesystem::path path;
};

struct node_configuration_t {
    std::string name;
    std::vector<native_module_configuration_t> native_module_configurations;
    std::unordered_map<std::string, platform_configuration_t> platform_configurations;
};

node_configuration_t load_configuration(std::string &&);
} // namespace interop
