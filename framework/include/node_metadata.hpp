#pragma once

#include <stdint.h>

namespace interop {

struct node_metadata_t {
    enum class architecture_e : uint8_t { x86, x64, arm, arm64 };
    enum class endianness_e : uint8_t { BigEndian, LittleEndian };

    architecture_e architecture;
    endianness_e endianness;

    node_metadata_t();
};

} // namespace interop
