#include <swizzle/IsType.hpp>
#include <regex>

namespace swizzle {

    namespace {
        static const std::regex keywords("u8|u16|u32|u64", std::regex::optimize);
    }
    
    bool IsUnsignedIntegerType(const boost::string_view& token)
    {
        return std::regex_match(token.begin(), token.end(), keywords);
    }
}
