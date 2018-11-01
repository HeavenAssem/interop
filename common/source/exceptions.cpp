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

open_error_t::open_error_t(string what)
  : error_t(move(what))
{}

validation_error_t::validation_error_t(string what)
  : error_t(move(what))
{}

module_validation_error_t::module_validation_error_t(string what)
  : validation_error_t(move(what))
{}

open_path_error_t::open_path_error_t(string what)
  : open_error_t(move(what))
{}

open_file_error_t::open_file_error_t(string what)
  : open_error_t(move(what))
{}

register_error_t::register_error_t(string what)
  : error_t(move(what))
{}

loading_error_t::loading_error_t(string what)
  : error_t(move(what))
{}

node_loading_error_t::node_loading_error_t(string what)
  : loading_error_t(move(what))
{}

module_loading_error_t::module_loading_error_t(string what)
  : loading_error_t(move(what))
{}

library_loading_error_t::library_loading_error_t(string what)
  : loading_error_t(move(what))
{}

symbol_loading_error_t::symbol_loading_error_t(string what)
  : loading_error_t(move(what))
{}

library_unloading_error_t::library_unloading_error_t(string what)
  : loading_error_t(move(what))
{}

lookup_error_t::lookup_error_t(string what)
  : error_t(move(what))
{}

module_lookup_error_t::module_lookup_error_t(string what)
  : lookup_error_t(move(what))
{}

function_lookup_error_t::function_lookup_error_t(string what)
  : lookup_error_t(move(what))
{}

type_lookup_error_t::type_lookup_error_t(string what)
  : lookup_error_t(move(what))
{}

function_call_error_t::function_call_error_t(string what)
  : error_t(move(what))
{}

arguments_mismatch_error_t::arguments_mismatch_error_t(string what)
  : error_t(move(what))
{}
} // namespace interop
