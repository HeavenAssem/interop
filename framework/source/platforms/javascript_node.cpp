//
// Created by islam on 31.12.17.
//

#include "platform.h"
#include "platform_factory.h"
#include <exceptions.h>

#include <node/libplatform/libplatform.h>
#include <node/node.hpp>

using namespace std;
using namespace v8;

namespace interop {
class platform_nodejs final: public platform {
  public:
    void initialize(const string & execution_path) override
    {
        throw not_implemented("nodejs platform is not implemented yet");
    }

    void run() override { throw not_implemented("nodejs platform is not implemented yet"); }

    void dispose() override { throw not_implemented("nodejs platform is not implemented yet"); }
};

register_platform_as("nodejs", platform_nodejs)
} // namespace interop
