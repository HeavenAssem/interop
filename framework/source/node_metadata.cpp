#include "node_metadata.hpp"

namespace interop {
namespace {

constexpr bool is_big_endian()
{
    union {
        uint32_t i;
        char c[4];
    } bint = {0x01020304};

    return bint.c[0] == 1;
}

} // namespace

node_metadata_t::node_metadata_t()
  :
#ifdef __i386__
  architecture(architecture_e::x86)
#elif __x86_64__
  architecture(architecture_e::x64)
#elif __arm__
  architecture(architecture_e::arm)
#elif __aarch64__
  architecture(architecture_e::arm64)
#endif
  , endianness(is_big_endian() ? endianness_e::BigEndian : endianness_e::LittleEndian)

{}

} // namespace interop
