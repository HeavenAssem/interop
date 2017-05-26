//
// Created by islam on 06.05.17.
//

#include "os.h"

#include "exceptions.h"


namespace mosaic {
    namespace os {
        library_handle load_library(const string_view & path) {
            throw mosaic::not_implemented("mosaic::os::load_library");
            return nullptr;
        }

        library_handle load_symbol(library_handle library, const string_view & name) {
            throw mosaic::not_implemented("mosaic::os::load_symbol");
            return nullptr;
        }

        void unload_library(library_handle library) {
            throw mosaic::not_implemented("mosaic::os::unload_library");
            return nullptr;
        }

        vector<string> walk(const string_view& path, const string_view& extension) {
            throw mosaic::not_implemented("mosaic::os::walk");
            return {};
        }
    }
}
