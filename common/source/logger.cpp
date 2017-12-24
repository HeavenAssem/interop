//
// Created by islam on 07.05.17.
//

#include "logger.h"

#include "rlutil.h"

#include <iostream>


using namespace std;

using namespace rlutil;

void print (const string_view & prefix, const string_view &message, const string_view &location, int color) {
    setColor(color);
    cout << prefix << ": " << "@" << location << ": " << message << endl;
    resetColor();
}

namespace mosaic {
    namespace logger {

        bool enabled = true;

        void set_enabled(bool e) {
            enabled = e;
        }

        void log(const string_view &message, const string_view& location) {
            if (!enabled) {
                return;
            }
            print("<mosaic> log", message, location, GREY);
        }

        void warning(const string_view &message, const string_view& location) {
            if (!enabled) {
                return;
            }
            print("<mosaic> warning", message, location, YELLOW);
        }

        void error(const string_view &message, const string_view& location) {
            if (!enabled) {
                return;
            }
            print("<mosaic> error", message, location, RED);
        }
    }
}