//
// Created by islam on 07.05.17.
//

#include "native_module.h"
#include "configuration.h"
#include "metadata_backward_compatibility.h"
#include "node.h"

#include "module_definitions.h"

#include <exceptions.h>
#include <logger.h>

#include <algorithm>

using namespace std;

namespace interop {
namespace {
version_t get_version(const shared_library & library)
{
    return *reinterpret_cast<version_t *>(library.symbol(INTEROP_FRAMEWORK_API_VERSION_STR));
}

module_metadata_t get_metadata(const shared_library & library)
{
    return convert_metadata_to_current(get_version(library),
                                       library.symbol(INTEROP_MODULE_METADATA_STR));
}
} // namespace

native_module_t::native_module_t(shared_library && _library,
                                 const native_module_configuration_t & configuration)
  : interop_abi_version(get_version(_library))
  , library(move(_library))
//, node(node)
{
    try {
        /** registration stage begin **/
        auto register_module =
            reinterpret_cast<register_module_function>(library.symbol(INTEROP_MODULE_REGISTER_STR));
        register_module();

        metadata = interop::get_metadata(library);

        if (!configuration.name.empty()) {
            metadata.name = configuration.name;
        } else if (metadata.name.empty()) {
            metadata.name = library.name();
        }

        validate_metadata();
        /** registration stage end **/
    } catch (const error_t & error) {
        throw module_loading_error_t("unable to load module from native library '" +
                                     library.name() + "': " + error.what());
    }
}

void native_module_t::link(node_t & node) const
{
    try {
        auto initialize_module = reinterpret_cast<initialize_module_function>(
            library.symbol(INTEROP_MODULE_INITIALIZE_STR));
        initialize_module(node);
    } catch (symbol_loading_error_t & e) {
        interop_logger(warning, e.what());
    }
}

native_module_t::native_module_t(native_module_t && other) noexcept
  : base_module_t(move(other))
  , library(move(other.library))
{}

native_module_t::~native_module_t() noexcept
{
    if (library) {
        interop_logger(warning,
                       "Module unload from destructor. Call unload() before destroying object.");
        try {
            unload();
        } catch (error_t & e) {
            interop_logger(error, e.what());
        }
    }
}

void native_module_t::unload()
{
    base::unload();

    library.unload();
}

const std::string & native_module_t::name() const { return metadata.name; }

void native_module_t::validate_metadata() const
{
    if (metadata.name.empty()) {
        throw module_validation_error_t("module name is empty");
    }
}

void native_module_t::listen(const std::string_view & module_name, std::function<void()> && handler)
{
    std::ignore = module_name;
    std::ignore = handler;
    throw not_implemented("native_module_t::listen");
}

function_ptr_t native_module_t::fetch_function(const std::string & name)
{
    auto it =
        find_if(metadata.functions.begin(), metadata.functions.end(),
                [&](const function_metadata_t & fn_metadata) { return name == fn_metadata.name; });
    if (it == metadata.functions.end()) {
        throw function_lookup_error_t("function with name \"" + name + "\" not found in module " +
                                      metadata.name);
    }

    return make_shared<function_view_t>(*it);
}
} // namespace interop
