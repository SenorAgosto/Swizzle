#include <swizzle/IsIntegerType.hpp>
#include <regex>

namespace swizzle {

    namespace {
        static const std::regex keywords("u8|i8|u16|i16|u32|i32|u64|i64", std::regex::optimize);
    }
    
    bool IsIntegerType(const boost::string_view& token)
    {
        return std::regex_match(token.begin(), token.end(), keywords);
    }
}
