#include "utils/typing.h"

#include <cxxabi.h>

using namespace std;

namespace interop {
namespace utils {
string demangle(const char * mangledName)
{
    int status;
    auto name = abi::__cxa_demangle(mangledName, nullptr, 0, &status);
    string result(name);
    free(name);
    return result;
}
} // namespace utils
} // namespace interop
