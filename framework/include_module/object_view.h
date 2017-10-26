#pragma once
//
// Created by islam on 26.10.17.
//

#include "object_metadata.h"
#include "declarations.h"


namespace mosaic {
    class object_view {
        const object_metadata & metadata;
        void * pointer;

    public:
        object_view(void * pointer, const object_metadata & metadata)
            : metadata(metadata)
            , pointer(pointer)
        {

        }

        function_ptr function(const std::string & name) const;

        void * get_pointer();

        ~object_view();
    };
}


