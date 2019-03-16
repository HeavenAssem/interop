#include "object_view.hpp"
#include "native_object.hpp"

using namespace std;

namespace interop {
object_ptr_t object_view_t::create(void * native_object, const object_metadata_t & metadata)
{
    return make_unique<native_object_t>(native_object, metadata);
}
} // namespace interop
