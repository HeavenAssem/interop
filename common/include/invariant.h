#pragma once

#include <cassert>
#include <sstream>

#define INTEROP_INVARIANT(CONDITION, MSG_EXPRESSION)                                               \
    do {                                                                                           \
        if (!(CONDITION)) {                                                                        \
            std::ostringstream ss;                                                                 \
            ss << MSG_EXPRESSION;                                                                  \
            throw std::logic_error(ss.str());                                                      \
        }                                                                                          \
    } while (false)
