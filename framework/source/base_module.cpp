//
// Created by islam on 08.04.18.
//

#include "base_module.h"
#include "logger.h"

#include <cassert>

using namespace std;

namespace mosaic {
    base_module_t::base_module_t()
    {

    }

    const module_metadata & base_module_t::get_metadata() const {
        return metadata;
    }

    function_ptr_t base_module_t::function(const string & name) {
        auto & function_ptr = functions_cache[name];
        if (!function_ptr) {
            function_ptr = fetch_function(name);
        }

        assert(function_ptr);

        return function_ptr;
    }

    void base_module_t::unload() {
        mosaic_logger(log, "unload module '" + name() + "'");

        functions_cache.clear();
    }
}
