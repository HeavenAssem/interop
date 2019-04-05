//
// Created by islam on 13.05.17.
//

#include <configuration.h>
#include <global.h>
#include <logger.hpp>
#include <node.h>

#include <gtest/gtest.h>

using namespace std;

int main(int argc, char ** argv)
{
    int c;
    string configPath;

    while ((c = getopt(argc, argv, "c:")) != -1) {
        //~ int this_option_optind = optind ? optind : 1;
        switch (c) {
        case 'c':
            configPath = optarg;
            break;
        default:
            break;
        }
    }

    if (configPath.empty()) {
        throw runtime_error("no config");
    }

    interop::logger::initialize(true);

    interop::node_t application(interop::load_configuration(move(configPath)));
    application.link();

    global::Testing = true;

    ::testing::InitGoogleTest(&argc, argv);

    auto exit_code = RUN_ALL_TESTS();

    application.unload();

    return exit_code;
}
