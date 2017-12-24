//
// Created by islam on 06.05.17.
//

#include "os.h"
#include "utility.h"

#include "exceptions.h"

#include <boost/algorithm/string/predicate.hpp>
#include <boost/filesystem/path.hpp>

#include <dlfcn.h>
#include <dirent.h>
#include <sys/stat.h>


using namespace std;


namespace mosaic {
    namespace os {
        library_handle load_library(const string_view & path) {
            if (path.empty()) {
                throw mosaic::library_loading_error("Failed to load library: empty path.");
            }

            auto handle = dlopen(path.data(), RTLD_LAZY);
            if (handle == NULL) {
                throw mosaic::library_loading_error(string("Failed to load library by path: \"") + path.data() +
                                                    "\"\nError: " + dlerror()
                );
            }
            return handle;
        }

        library_handle load_symbol(library_handle library, const string_view & name) {
            auto pointer = dlsym(library, name.data());
            if (pointer == NULL) {
                throw mosaic::symbol_loading_error(string("Failed to load symbol with name: \"") + name.data() +
                                                   "\"\nError: " + dlerror()
                );
            }
            return pointer;
        }

        void unload_library(library_handle library) {
            if (dlclose(library) != 0) {
                throw mosaic::library_unloading_error(string("Failed to unload library.") +
                                                      "\"\nError: " + dlerror()
                );
            }
        }

        vector<string> walk(const string_view& path, const string_view& extension, bool file, bool directory) {
            vector<string> files;

            DIR *dp;
            dirent *dirp;
            if ((dp = opendir(path.data())) == NULL) {
                auto error = read_errno();
                throw mosaic::open_path_error(string("Failed to open directory by path \"") + path.data() + "\": " + error.data());
            }

            while ((dirp = readdir(dp)) != NULL) {
                bool is_directory;
                if (dirp->d_type == DT_UNKNOWN) {
                    struct stat buf;
                    stat(dirp->d_name, &buf);
                    is_directory = S_ISDIR(buf.st_mode);
                }
                else {
                    is_directory = dirp->d_type == DT_DIR;
                }

                if ((!directory && is_directory) || (!file && !is_directory)) {
                    continue;
                }

                if (boost::algorithm::ends_with(dirp->d_name, extension.data())) {
                    if (string(".") != dirp->d_name && string("..") != dirp->d_name) {
                        boost::filesystem::path boost_path(path.data());
                        boost_path.append(dirp->d_name);
                        files.push_back(boost_path.string());
                    }
                }
            }
            closedir(dp);
            return files;
        }

        std::string_view library_extension = ".so";
    }
}
