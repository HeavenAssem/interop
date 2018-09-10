//
// Created by islam on 06.05.17.
//

#include "os.h"

#include "exceptions.h"


namespace interop {
    namespace os {
        library_handle load_library(const string_view & path) {
            throw interop::not_implemented("interop::os::load_library");
            return nullptr;
        }

        library_handle load_symbol(library_handle library, const string_view & name) {
            throw interop::not_implemented("interop::os::load_symbol");
            return nullptr;
        }

        void unload_library(library_handle library) {
            throw interop::not_implemented("interop::os::unload_library");
            return nullptr;
        }

        vector<string> walk(const string_view& path, const string_view& extension) {
            throw interop::not_implemented("interop::os::walk");
            return {};
        }

        std::string_view library_extension = ".dll";
    }
}
