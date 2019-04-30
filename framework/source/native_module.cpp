//
// Created by islam on 07.05.17.
//

#include "native_module.h"
#include "configuration.h"
#include "metadata_backward_compatibility.h"
#include "native_function.hpp"
#include "node.hpp"

#include "module_definitions.h"

#include <exceptions.h>
#include <logger.hpp>

#include <boost/scope_exit.hpp>

#include <algorithm>

using namespace std;

namespace interop {
namespace {
version_t get_version(const shared_library_t & library)
{
    return library.symbol_as<version_t>(interop_framework_abi_version_c);
}

module_metadata_t register_module_and_get_metadata(const shared_library_t & library)
{
    auto register_module = library.symbol_as<register_module_function>(interop_module_register_c);
    register_module();

    return convert_metadata_to_current(get_version(library),
                                       library.symbol(interop_module_metadata_c));
}
} // namespace

native_module_t::native_module_t(module_metadata_t metadata, shared_library_t library_p)
  : internal_module_t(metadata.name, std::move(metadata))
  , library(std::move(library_p))
  , interop_abi_version(get_version(library))
{}

native_module_t::native_module_t(shared_library_t library_p,
                                 const native_module_configuration_t & configuration)
try: native_module_t(register_module_and_get_metadata(library_p), std::move(library_p)) {
    if (!configuration.name.empty()) {
        // override name with configured one regardless
        name = configuration.name;
    } else if (name.empty()) {
        // if no name - fallback to library name
        name = library.name();
    }

    validate_metadata();
} catch (const error_t & error) {
    throw module_loading_error_t("unable to load module from native library '" + library_p.name() +
                                 "': " + error.what());
}

native_module_t::classes_sequence_t native_module_t::iterate_classes() const
{
    return [this, class_id = class_id_t{get_id(), {}}]() mutable -> classes_sequence_t::value_t {
        BOOST_SCOPE_EXIT(&class_id) { ++class_id.local_id; }
        BOOST_SCOPE_EXIT_END

        if (class_id.local_id == native->types.size()) {
            return {};
        }

        return {{class_id, native->types[class_id.local_id]}};
    };
}

native_module_t::functions_sequence_t native_module_t::iterate_functions() const
{
    return native->functions;
}

const object_metadata_t & native_module_t::get_object_metadata(class_id_t class_id) const
{
    using namespace std::string_literals;

    interop_invariant_m(class_id.module_id == get_id(),
                        "accessing metadata of class from another module");
    interop_invariant_m(class_id.local_id < native->types.size(),
                        "invalid class id: " << class_id.local_id << ": out of range");

    return native->types[class_id.local_id];
}

void native_module_t::link(node_t & node)
{
    try {
        auto initialize_module =
            reinterpret_cast<initialize_module_function>(library.symbol(interop_module_link_c));
        initialize_module(node);
    } catch (symbol_loading_error_t & e) {
        interop_logger(warning, e.what());
    }
}

object_ptr_t native_module_t::create_dynamic(const std::string_view & name, arg_pack_t args)
{
    const auto & metadata = module_view_t::get_object_metadata(name);

    interop_invariant_m(!metadata.constructors.empty(), "empty constructors past validation");

    std::ostringstream ss;

    for (const auto & constructor_metadata : metadata.constructors) {
        try {
            return object_view_t::create(
                native_function_t::call_dynamic(constructor_metadata.invoke,
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
  : internal_module_t(move(other))
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

void native_module_t::validate_metadata() const
{
    if (get_name().empty()) {
        throw module_validation_error_t("module name is empty");
    }

    std::ostringstream ss;
    size_t objects_with_no_name = 0;

    for (const auto & object_metadata : native->types) {
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
        find_if(native->functions.begin(), native->functions.end(),
                [&](const function_metadata_t & fn_metadata) { return name == fn_metadata.name; });
    if (it == native->functions.end()) {
        throw function_lookup_error_t("function with name \""s + name.data() +
                                      "\" not found in module " + get_name());
    }

    return make_shared<native_function_t>(*it);
}
} // namespace interop
