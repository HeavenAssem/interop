//
// Created by islam on 13.05.17.
//

#pragma once

#include "function_view.h"
#include "object_view.h"

#include "definitions.h"
#include "module_metadata.h"

#include <functional>


namespace mosaic {
    class module_view {
        virtual const module_metadata & get_metadata() const = 0;

    public:
        virtual function_ptr function(const std::string & name) const = 0;
        template <typename ... Args>
        object_ptr create(const std::string & name, Args && ... args) const {
            auto & metadata = get_metadata();

            auto it = std::find_if(metadata.objects.begin(), metadata.objects.end(), [&](const object_metadata & o_metadata){
                return name == o_metadata.name;
            });
            if (it == metadata.objects.end()) {
                throw object_lookup_error("function with name \"" + name + "\" not found in module " + metadata.name);
            }

            void * object = reinterpret_cast<void * (*)(Args && ...)>(it->constructors[0].pointer)(std::forward<Args>(args)...);

            return std::make_shared<object_view>(object, *it);
        }
        virtual void listen(const std::string_view & module_name, const std::function<void()> handler) = 0;
        virtual const std::string& name() const = 0;
    };
}
