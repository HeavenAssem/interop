#include "configuration.h"
#include "exceptions.h"

#include <boost/algorithm/string.hpp>
#include <json/json.h>

#include <fstream>

using namespace std;

namespace interop {
node_configuration_t load_configuration(string && filename)
{
    Json::Value root;
    {
        boost::trim(filename);

        ifstream config_file(filename);

        if (!config_file.is_open()) {
            throw open_file_error_t("unable to open configuration file: " + filename);
        }

        Json::CharReaderBuilder builder;
        JSONCPP_STRING errors;
        Json::parseFromStream(builder, config_file, &root, &errors);
    }

    node_configuration_t configuration;
    {
        configuration.name = root.get("name", "<unspecified>").asString();

        const auto & platforms = root.get("platforms", Json::objectValue);
        if (!platforms.isObject()) {
            throw validation_error_t("invalid JSON config: 'platforms' must be object");
        }

        configuration.platform_configurations.reserve(platforms.size());

        for (auto platform_value_it = platforms.begin(); platform_value_it != platforms.end();
             ++platform_value_it) {
            const auto & key   = platform_value_it.key();
            const auto & value = *platform_value_it;

            auto & platform_configuration = configuration.platform_configurations[key.asString()];

            platform_configuration.execution_path = value.get("execution_path", "").asString();

            const auto & modules = value.get("modules", Json::arrayValue);
            if (!modules.isArray()) {
                throw validation_error_t("invalid JSON config: 'modules' must be array");
            }

            for (const auto & module : modules) {
                platform_configuration.module_configurations.emplace_back();
                auto & module_configuration = platform_configuration.module_configurations.back();

                module_configuration.name = module.get("name", "<unspecified>").asString();

                const auto & paths = module.get("paths", Json::arrayValue);
                if (!paths.isArray()) {
                    throw validation_error_t("invalid JSON config: 'paths' must be array");
                }

                for (const auto & path : paths) {
                    module_configuration.paths.push_back(path.asString());
                }
            }
        }

        const auto & native_modules = root.get("native_modules", Json::arrayValue);
        if (!native_modules.isArray()) {
            throw validation_error_t("invalid JSON config: 'native_modules' must be array");
        }

        configuration.native_module_configurations.reserve(native_modules.size());

        for (const auto & native_module : native_modules) {
            configuration.native_module_configurations.push_back(
                {native_module.get("name", "").asString(),
                 native_module.get("path", "").asString()});
        }
    }

    return configuration;
}
} // namespace interop
