//
// Created by islam on 11.05.17.
//

#pragma once


#include "definitions.h"

#include <cerrno>
#include <cstring>


std::string_view read_errno() {
    return strerror(errno);
}
