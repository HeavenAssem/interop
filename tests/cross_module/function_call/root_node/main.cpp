//
// Created by islam on 13.05.17.
//

#include <node.h>


int main() {
    mosaic::node application("../");
    application.link();
    application.unload();
}
