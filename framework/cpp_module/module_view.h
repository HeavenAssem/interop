//
// Created by islam on 13.05.17.
//

#pragma once

#include "function_view.h"
#include "object_view.h"

#include "definitions.h"
#include "module_metadata.h"

#include <functional>
#include <declarations.h>


namespace mosaic {
    class module_view {
        virtual const module_metadata & get_metadata() const = 0;

        const object_metadata & get_object_metadata(const std::string & name) const {
            auto & metadata = get_metadata();

            auto it = std::find_if(metadata.types.begin(), metadata.types.end(), [&](const object_metadata & o_metadata){
                return name == o_metadata.name;
            });
            if (it == metadata.types.end()) {
                throw type_lookup_error("type with name \"" + name + "\" not found in module \"" + metadata.name + "\"");
            }
            return *it;
        }

    public:
        virtual function_ptr_t function(const std::string & name) = 0;

        template <typename ... Args>
        object_ptr create(const std::string & name, Args && ... args) const {
            auto & metadata = get_object_metadata(name);

            auto it = std::find_if(metadata.constructors.begin(), metadata.constructors.end(), [](const constructor_metadata & c_metadata){
                try {
                    detail::signature_checker<void, Args...>::check_args(c_metadata.arguments);
                } catch (const arguments_mismatch_error &) {
                    return false;
                }
                return true;
            });

            if (it == metadata.constructors.end()) {
                throw function_lookup_error("no matching constructor in type " + name);
            }

            void * object = reinterpret_cast<void * (*)(Args && ...)>(it->pointer)(std::forward<Args>(args)...);

            return std::make_shared<object_view>(object, metadata);
        }
        virtual void listen(const std::string_view & module_name, std::function<void()> && handler) = 0;
        virtual const std::string& name() const = 0;

        virtual ~module_view() = default;
    };
}
