//
// Created by islam on 06.05.17.
//

#pragma once

#include "definitions.h"

#include <vector>


namespace mosaic {
    namespace os {
        /**
         * @brief loads dynamic library (.dll for windows, .so for unix)
         * @param path: path to .so or .dll file
         * @returns handle (base address) of loaded library
         * @throws mosaic::library_loading_error if loading failed
         */
        library_handle load_library(const std::string_view & path);

        /**
         * @brief loads symbol from dynamic library
         * @param library: handle (base address) of loaded library
         * @param name: name of symbol to load from library
         * @returns pointer to symbol
         * @throws mosaic::symbol_loading_error if loading failed
         */
        library_symbol_handle load_symbol(library_handle library, const std::string_view & name);

        /**
         * @brief unloads dynamic library from address space (os - dependent)
         * @param library: handle (base address) of loaded library to be unloaded
         * @throws mosaic::library_unloading_error if unloading failed
         */
        void unload_library(library_handle library);

        /**
         * @brief lists all items by path
         * @param path: path where to look for items
         * @param extension: file extension preceded by dot (".so", ".dll") [optional, default: empty string]
         * @param file: include files [optional, default: true]
         * @param directory: include directories [optional, default: false]
         * @returns vector of items names
         * @throws mosaic::open_path_error if couldn't open path
         */
        std::vector<std::string> walk(const std::string_view& path, const std::string_view& extension = "", bool file = true, bool directory = false);
    }
}
