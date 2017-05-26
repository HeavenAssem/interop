#pragma once
//
// Created by islam on 06.05.17.
//

#include "definitions.h"


namespace mosaic {
    /**
     * @class shared_library
     * @brief encapsulates shared library, manages its lifetime
     * @attention non default - constructable; non copyable; movable
     */
    class shared_library {
        library_handle  handle;
        std::string     library_name;
    public:
        explicit shared_library(const std::string_view & path, const std::string_view & name = "");

        shared_library(const shared_library &) = delete;

        shared_library(shared_library &&);

        ~shared_library();

        library_symbol_handle symbol(const std::string_view & name) const;

        const std::string & name() const;

        void unload();

    private:
        void reset();
    };
}


