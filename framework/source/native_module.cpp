//
// Created by islam on 07.05.17.
//

#include "native_module.h"
#include "configuration.h"
#include "metadata_backward_compatibility.h"
#include "native_function.hpp"
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
    return *reinterpret_cast<version_t *>(library.symbol(interop_framework_abi_version_c));
}

module_metadata_t get_metadata(const shared_library & library)
{
    return convert_metadata_to_current(get_version(library),
                                       library.symbol(interop_module_metadata_c));
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
            reinterpret_cast<register_module_function>(library.symbol(interop_module_register_c));
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
        auto initialize_module =
            reinterpret_cast<initialize_module_function>(library.symbol(interop_module_link_c));
        initialize_module(node);
    } catch (symbol_loading_error_t & e) {
        interop_logger(warning, e.what());
    }
}

object_ptr_t native_module_t::create_dynamic(const std::string_view & name, arg_pack_t args) const
{
    const auto & metadata = get_object_metadata(name);

    interop_invariant_m(!metadata.constructors.empty(), "empty constructors past validation");

    std::ostringstream ss;

    for (const auto & constructor_metadata : metadata.constructors) {
        try {
            return object_view_t::create(
                native_function_t::dynamic_call(constructor_metadata.invoke,
                                                constructor_metadata.pointer, args,
                                                constructor_metadata.arguments)
                    .as<void *>(),
                metadata);
        } catch (const typing_error_t & e) {
            ss << "candidate failed: " << e.what() << ";";
        }
    }

    throw function_lookup_error_t("unable to find suitable overload for " + metadata.name +
                                  " constructor: " + ss.str());
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

    std::ostringstream ss;
    size_t objects_with_no_name = 0;

    for (const auto & object_metadata : metadata.types) {
        if (object_metadata.name.empty()) {
            ++objects_with_no_name;
        } else if (object_metadata.constructors.empty()) {
            ss << object_metadata.name << " has no constructors; ";
        }
    }

    if (objects_with_no_name) {
        ss << objects_with_no_name << " objects have no name; ";
    }

    const auto & errors = ss.str();

    if (!errors.empty()) {
        throw module_validation_error_t(errors);
    }
}

void native_module_t::listen(const std::string_view & module_name, std::function<void()> && handler)
{
    std::ignore = module_name;
    std::ignore = handler;
    throw not_implemented("native_module_t::listen");
}

function_ptr_t native_module_t::fetch_function(const std::string_view & name)
{
    auto it =
        find_if(metadata.functions.begin(), metadata.functions.end(),
                [&](const function_metadata_t & fn_metadata) { return name == fn_metadata.name; });
    if (it == metadata.functions.end()) {
        throw function_lookup_error_t("function with name \""s + name.data() +
                                      "\" not found in module " + metadata.name);
    }

    return make_shared<native_function_t>(*it);
}
} // namespace interop
