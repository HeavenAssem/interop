//
// Created by islam on 06.05.17.
//

#include "os.h"
#include "utility.h"

#include "exceptions.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/path.hpp>

#include <dirent.h>
#include <dlfcn.h>
#include <logger.hpp>
#include <queue>
#include <sys/stat.h>

using namespace std;

namespace interop {
namespace os {
library_handle load_library(const string_view & path)
{
    if (path.empty()) {
        throw interop::library_loading_error_t("Failed to load library: empty path.");
    }

    auto handle = dlopen(path.data(), RTLD_LAZY);
    if (handle == NULL) {
        throw interop::library_loading_error_t(string("Failed to load library by path: \"") +
                                               path.data() + "\"\nError: " + dlerror());
    }
    return handle;
}

library_handle load_symbol(library_handle library, const string_view & name)
{
    auto pointer = dlsym(library, name.data());
    if (pointer == NULL) {
        throw interop::symbol_loading_error_t(string("Failed to load symbol with name: \"") +
                                              name.data() + "\"\nError: " + dlerror());
    }
    return pointer;
}

void unload_library(library_handle library)
{
    if (dlclose(library) != 0) {
        throw interop::library_unloading_error_t(string("Failed to unload library.") +
                                                 "\"\nError: " + dlerror());
    }
}

lazy_sequence_t<string> walk(const string_view & path, const string_view & extension,
                             walk_policy_e policy, entry_type_e entry_type)
{
    DIR * dp;
    interop_logger(log, "open directory: " + string(path));
    if ((dp = opendir(path.data())) == nullptr) {
        auto error = read_errno();
        throw interop::open_path_error_t(string("Failed to open directory by path \"") +
                                         path.data() + "\": " + error.data());
    }

    const auto file      = static_cast<bool>(entry_type & File);
    const auto directory = static_cast<bool>(entry_type & Directory);

    queue<pair<DIR *, boost::filesystem::path>> dirs;
    dirs.push({dp, boost::filesystem::path(path.data())});

    return lazy_sequence_t<string>([=]() mutable {
        while (!dirs.empty()) {
            auto [dir, base_path] = dirs.front();

            dirent * entry;

            while ((entry = readdir(dir)) != nullptr) {
                bool is_directory;
                if (entry->d_type == DT_UNKNOWN) {
                    is_directory = get_entry_type(entry->d_name) == Directory;
                } else {
                    is_directory = entry->d_type == DT_DIR;
                }

                if (string(".") == entry->d_name || string("..") == entry->d_name) {
                    continue;
                }

                if (policy == Recursive && is_directory) {
                    auto dir_path = base_path / entry->d_name;
                    interop_logger(log, "open directory: " + dir_path.string());
                    dirs.push({opendir(dir_path.c_str()), dir_path});
                }

                if ((!directory && is_directory) || (!file && !is_directory)) {
                    continue;
                }

                if (boost::algorithm::ends_with(entry->d_name, extension.data())) {
                    return (base_path / entry->d_name).string();
                }
            }

            dirs.pop();
            closedir(dir);
        }

        throw lazy_sequence_end_t();
    });
}

entry_type_e get_entry_type(const string_view & path)
{
    struct stat buf;
    stat(path.data(), &buf);

    return S_ISDIR(buf.st_mode) ? Directory : File;
}

string_view library_extension = ".so";
} // namespace os
} // namespace interop
