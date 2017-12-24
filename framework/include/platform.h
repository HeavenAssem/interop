//
// Created by islam on 07.11.17.
//

#pragma once

#include <string>

namespace mosaic {
    class platform {
    public:
        virtual ~platform() = default;

        virtual void initialize(const std::string & execution_path) = 0;
        virtual void run() = 0;
        virtual void dispose() = 0;
    };
}
