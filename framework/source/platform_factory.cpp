//
// Created by islam on 24.12.17.
//

#include "platform_factory.h"
#include "exceptions.h"
#include "platform.h"

#include <unordered_map>

using namespace std;

namespace interop {
unordered_map<string, platform_factory_ptr> platforms;

platform_registrator::platform_registrator(const std::string & id, platform_factory_ptr platform)
{
    auto & stored_platform = platforms[id];
    if (stored_platform) {
        throw platform_loading_error_t("name collision: platform  " + id + " already registered");
    }

    stored_platform = move(platform);
}

platform_ptr_t instantiate_platform(const std::string & id)
{
    if (const auto & wrapped_platform = platforms[id]) {
        return wrapped_platform->instantiate();
    } else {
        throw not_implemented("platform '" + id + "' is not implemented");
    }
}
} // namespace interop
