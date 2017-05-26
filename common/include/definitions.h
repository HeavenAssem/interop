//
// Created by islam on 07.05.17.
//

#pragma once

#include <experimental/string_view>

namespace std {
    typedef experimental::string_view string_view;
}

namespace mosaic {
    typedef void * library_handle;
    typedef void * library_symbol_handle;
}
