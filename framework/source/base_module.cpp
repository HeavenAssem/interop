//
// Created by islam on 08.04.18.
//

#include "base_module.h"
#include "logger.hpp"

#include <cassert>

using namespace std;
using namespace placeholders;

namespace interop {
base_module_t::base_module_t() {}

const module_metadata_t & base_module_t::get_metadata() const { return metadata; }

function_ptr_t base_module_t::function(const string_view & name)
{
    return cache.get(name, bind(&base_module_t::fetch_function, this, _1));
}

void base_module_t::unload()
{
    interop_logger(log, "unload module '" + name() + "'");

    cache.clear();
}
} // namespace interop
