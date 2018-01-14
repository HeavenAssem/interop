//
// Created by islam on 07.05.17.
//

#include "module.h"
#include "node.h"
#include "metadata_backward_compatibility.h"

#include "module_definitions.h"

#include <exceptions.h>
#include <logger.h>

#include <algorithm>

using namespace std;


namespace mosaic {
    module::module(shared_library && _library): library(move(_library)) {
        try {
            /** registration stage begin **/
            auto register_module = reinterpret_cast<register_module_function>(library.symbol(MOSAIC_MODULE_REGISTER_STR));
            register_module();

            mosaic_api_version = *reinterpret_cast<version_t*>(library.symbol(MOSAIC_FRAMEWORK_API_VERSION_STR));

            /** version of module's mosaic API matches version of node **/
            if (MOSAIC_FRAMEWORK_API_VERSION == mosaic_api_version) {
                metadata = *reinterpret_cast<module_metadata *>(library.symbol(MOSAIC_MODULE_METADATA_STR));
            } else {
                metadata = convert_metadata_to_current(mosaic_api_version, library.symbol(MOSAIC_MODULE_METADATA_STR));
            }

            if (metadata.name.empty()) {
                metadata.name = library.name();
            }
            validate_metadata();
            /** registration stage end **/
        } catch (const error_t & error) {
            throw module_loading_error("unable to load module from native library '" + library.name() + "': " + error.what());
        }
    }

    void module::link(module_context & context) const {
        try {
            auto initialize_module = reinterpret_cast<initialize_module_function>(library.symbol(MOSAIC_MODULE_INITIALIZE_STR));
            initialize_module(context);
        } catch (symbol_loading_error & e) {
            mosaic_logger(warning, e.what());
        }
    }

    module::module(module && other) noexcept
        : library(move(other.library))
    {
        metadata = other.metadata;

        other.metadata = {};
    }

    module::~module() noexcept {
        if (library) {
            mosaic_logger(warning, "Module unload from destructor. Call unload() before destroying object.");
            try {
                unload();
            } catch (error_t & e) {
                mosaic_logger(error, e.what());
            }
        }
    }

    void module::unload() {
        library.unload();
    }

    const std::string & module::name() const {
        return metadata.name;
    }

    void module::validate_metadata() const {
        if (metadata.name.empty()) {
            throw module_validation_error("module name is empty");
        }
    }

    function_ptr module::function(const string & name) const {
        auto used_function = used_functions[name];
        if (!used_function) {
            auto it = find_if(metadata.functions.begin(), metadata.functions.end(), [&](const function_metadata & fn_metadata){
                return name == fn_metadata.name;
            });
            if (it == metadata.functions.end()) {
                throw function_lookup_error("function with name \"" + name + "\" not found in module " + metadata.name);
            }

            used_function = make_shared<function_view>(*it);
        }

        return used_function;
    }

    void module::listen(const std::string_view & module_name, std::function<void()> && handler) {

    }

    const module_metadata & module::get_metadata() const {
        return metadata;
    }
}
