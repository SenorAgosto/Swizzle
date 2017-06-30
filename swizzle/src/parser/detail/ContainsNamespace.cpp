#include "./ContainsNamespace.hpp"
#include <regex>

namespace swizzle { namespace parser { namespace detail {

    namespace {
        static const std::regex colons("::", std::regex::optimize);
    }
    
    bool containsNamespace(const boost::string_view& type)
    {
        return std::regex_search(type.begin(), type.end(), colons);
    }
}}}


