//
// Created by islam on 06.05.17.
//

#include "exceptions.h"

using namespace std;

namespace interop {
error_t::error_t(string what)
  : message(move(move(what)))
{}

void error_t::append(const string_view & message) { this->message += message; }

const char * error_t::what() const noexcept { return message.c_str(); }
} // namespace interop
