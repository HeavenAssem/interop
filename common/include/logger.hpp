#pragma once
//
// Created by islam on 07.05.17.
//

#include "definitions.h"

namespace interop {
namespace logger {
void initialize(bool enabled = true);
void debug(const std::string_view & message, const std::string_view & location);
void log(const std::string_view & message, const std::string_view & location);
void warning(const std::string_view & message, const std::string_view & location);
void error(const std::string_view & message, const std::string_view & location);
} // namespace logger
} // namespace interop

#define interop_logger(type, message) interop::logger::type(message, __PRETTY_FUNCTION__)
