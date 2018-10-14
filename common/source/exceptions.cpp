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

not_implemented::not_implemented(string what)
  : error_t(move(what))
{}

open_error::open_error(string what)
  : error_t(move(what))
{}

validation_error::validation_error(string what)
  : error_t(move(what))
{}

module_validation_error::module_validation_error(string what)
  : validation_error(move(what))
{}

open_path_error::open_path_error(string what)
  : open_error(move(what))
{}

open_file_error::open_file_error(string what)
  : open_error(move(what))
{}

register_error_t::register_error_t(string what)
  : error_t(move(what))
{}

loading_error::loading_error(string what)
  : error_t(move(what))
{}

node_loading_error::node_loading_error(string what)
  : loading_error(move(what))
{}

module_loading_error::module_loading_error(string what)
  : loading_error(move(what))
{}

library_loading_error::library_loading_error(string what)
  : loading_error(move(what))
{}

symbol_loading_error::symbol_loading_error(string what)
  : loading_error(move(what))
{}

library_unloading_error::library_unloading_error(string what)
  : loading_error(move(what))
{}

lookup_error::lookup_error(string what)
  : error_t(move(what))
{}

module_lookup_error::module_lookup_error(string what)
  : lookup_error(move(what))
{}

function_lookup_error::function_lookup_error(string what)
  : lookup_error(move(what))
{}

type_lookup_error::type_lookup_error(string what)
  : lookup_error(move(what))
{}

function_call_error::function_call_error(string what)
  : error_t(move(what))
{}

arguments_mismatch_error::arguments_mismatch_error(string what)
  : error_t(move(what))
{}

platform_loading_error::platform_loading_error(string what)
  : loading_error(move(what))
{}
} // namespace interop
