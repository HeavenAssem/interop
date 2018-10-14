//
// Created by islam on 06.05.17.
//

#include "shared_library.h"
#include "global.h"

#include "exceptions.h"
#include "logger.h"
#include "os.h"

#include <boost/filesystem/path.hpp>

using namespace std;

namespace interop {
shared_library::shared_library(const std::string_view & path, const std::string_view & name)
{
    this->handle = os::load_library(path);
    this->library_name =
        name.empty() ? boost::filesystem::path(path.data()).stem().string() : name.data();
    interop_logger(log, "Loaded shared library \"" + this->library_name + "\" by path \"" +
                            path.data() + "\"");
}

shared_library::shared_library(shared_library && other) noexcept
{
    this->handle       = other.handle;
    this->library_name = other.library_name;

    other.reset();
}

shared_library::~shared_library()
{
    if (handle) {
        interop_logger(
            warning,
            "Shared library unload from destructor. Call unload() before destroying object.");
        try {
            unload();
        } catch (error_t & e) {
            interop_logger(error, e.what());
        }
    }
}

library_symbol_handle shared_library::symbol(const string_view & name) const
{
    return os::load_symbol(handle, name);
}

const std::string & shared_library::name() const { return library_name; }

void shared_library::unload()
{
    if (handle) {
        try {
            if (!global::Testing) { // manual unload of shared library containing tests will cause
                                    // SEGFAULT in GoogleTest
                os::unload_library(handle);
            }
            interop_logger(log, "Unloaded shared library \"" + this->library_name + "\"");
            reset();
        } catch (library_unloading_error & error) {
            if (!library_name.empty()) {
                error.append("\nLibrary name: " + library_name);
            }
            throw;
        }
    }
}

void shared_library::reset() noexcept
{
    handle       = nullptr;
    library_name = "unknown";
}

shared_library::operator bool() const noexcept { return bool(handle); }

bool shared_library::operator!() const noexcept { return !bool(*this); }
} // namespace interop
