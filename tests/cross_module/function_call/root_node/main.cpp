//
// Created by islam on 13.05.17.
//

#include <node.h>


int main() {
    mosaic::node application("../node");
    application.link();
    application.unload();
}
