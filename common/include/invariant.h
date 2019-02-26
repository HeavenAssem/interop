#pragma once

#include <sstream>

#define interop_invariant_m(CONDITION, MSG_EXPRESSION)                                             \
    do {                                                                                           \
        if (!(CONDITION)) {                                                                        \
            std::ostringstream ss;                                                                 \
            ss << "@" << __FILE__ << ":" << __LINE__ << MSG_EXPRESSION;                            \
            throw std::logic_error(ss.str());                                                      \
        }                                                                                          \
    } while (false)
