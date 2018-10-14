//
// Created by islam on 14.01.18.
//

#pragma once

#include "module_metadata.h"

namespace interop {
module_metadata_t convert_metadata_to_current(const version_t & version, const void * metadata);
}
