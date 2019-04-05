//
// Created by islam on 07.05.17.
//

#include "logger.hpp"

#include "rlutil.h"

#include <iostream>

using namespace std;

using namespace rlutil;

void print(const string_view & prefix, const string_view & message, const string_view & location,
           int color)
{
    cout << getANSIColor(color) << prefix << ": "
         << "@" << location << ": " << message << ANSI_ATTRIBUTE_RESET << endl;
}

namespace interop {
namespace logger {

bool enabled = true;

void initialize(bool e)
{
    if ((enabled = e)) {
        saveDefaultColor();
    }
}

void debug(const string_view & message, const string_view & location)
{
    if (!enabled) {
        return;
    }
    print("<interop> debug", message, location, DARKGREY);
}

void log(const string_view & message, const string_view & location)
{
    if (!enabled) {
        return;
    }
    print("<interop> log", message, location, GREY);
}

void warning(const string_view & message, const string_view & location)
{
    if (!enabled) {
        return;
    }
    print("<interop> warning", message, location, YELLOW);
}

void error(const string_view & message, const string_view & location)
{
    if (!enabled) {
        return;
    }
    print("<interop> error", message, location, RED);
}
} // namespace logger
} // namespace interop
