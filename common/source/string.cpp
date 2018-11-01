#include "utils/string.h"

#include <boost/algorithm/string/regex.hpp>

using namespace std;

namespace interop {
namespace utils {
vector<string> split_rx(const string & str, const string & del)
{
    vector<string> res;
    boost::algorithm::split_regex(res, str, boost::regex(del));
    return res;
}
} // namespace utils
} // namespace interop
