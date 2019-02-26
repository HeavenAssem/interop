#include "caches.hpp"
#include "function_view.hpp"

namespace interop
{
const std::string & function_ptr_key_getter_t::get_persistent_key(const function_ptr_t& function) {
      return function->get_metadata().name;
  }
}
