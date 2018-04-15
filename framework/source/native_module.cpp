//
// Created by islam on 07.05.17.
//

#include "native_module.h"
#include "node.h"
#include "configuration.h"
#include "metadata_backward_compatibility.h"

#include "module_definitions.h"

#include <exceptions.h>
#include <logger.h>

#include <algorithm>

using namespace std;


namespace mosaic {
    namespace {
        version_t get_version(const shared_library & library) {
            return *reinterpret_cast<version_t*>(library.symbol(MOSAIC_FRAMEWORK_API_VERSION_STR));
        }

        module_metadata get_metadata(const shared_library & library) {
            return convert_metadata_to_current(get_version(library), library.symbol(MOSAIC_MODULE_METADATA_STR));
        }
    }

    native_module_t::native_module_t(shared_library && _library, const native_module_configuration_t & configuration)
        : mosaic_abi_version(get_version(_library))
        , library(move(_library))
        //, node(node)
    {
        try {
            /** registration stage begin **/
            auto register_module = reinterpret_cast<register_module_function>(library.symbol(MOSAIC_MODULE_REGISTER_STR));
            register_module();

            metadata = mosaic::get_metadata(library);

            if (!configuration.name.empty()) {
                metadata.name = configuration.name;
            } else if (metadata.name.empty()) {
                metadata.name = library.name();
            }

            validate_metadata();
            /** registration stage end **/
        } catch (const error_t & error) {
            throw module_loading_error("unable to load module from native library '" + library.name() + "': " + error.what());
        }
    }

    void native_module_t::link(module_context & context) const {
        try {
            auto initialize_module = reinterpret_cast<initialize_module_function>(library.symbol(MOSAIC_MODULE_INITIALIZE_STR));
            initialize_module(context);
        } catch (symbol_loading_error & e) {
            mosaic_logger(warning, e.what());
        }
    }

    native_module_t::native_module_t(native_module_t && other) noexcept
        : base_module_t(move(other))
        , library(move(other.library))
    {}

    native_module_t::~native_module_t() noexcept {
        if (library) {
            mosaic_logger(warning, "Module unload from destructor. Call unload() before destroying object.");
            try {
                unload();
            } catch (error_t & e) {
                mosaic_logger(error, e.what());
            }
        }
    }

    void native_module_t::unload() {
        base::unload();

        library.unload();
    }

    const std::string & native_module_t::name() const {
        return metadata.name;
    }

    void native_module_t::validate_metadata() const {
        if (metadata.name.empty()) {
            throw module_validation_error("module name is empty");
        }
    }

    void native_module_t::listen(const std::string_view & module_name, std::function<void()> && handler) {

    }

    function_ptr_t native_module_t::create_function(const std::string & name) {
        auto it = find_if(metadata.functions.begin(), metadata.functions.end(), [&](const function_metadata & fn_metadata){
            return name == fn_metadata.name;
        });
        if (it == metadata.functions.end()) {
            throw function_lookup_error("function with name \"" + name + "\" not found in module " + metadata.name);
        }

        return make_shared<function_view>(*it);
    }
}
