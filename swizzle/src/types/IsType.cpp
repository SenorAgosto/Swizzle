#include <swizzle/types/IsType.hpp>
#include <regex>

namespace swizzle { namespace types {

    namespace {
        static const std::regex keywords("u8|i8|u16|i16|u32|i32|u64|i64|bool|bitfield|f32|f64|variable_block", std::regex::optimize);
    }
    
    bool IsType(const boost::string_view& token)
    {
        return std::regex_match(token.begin(), token.end(), keywords);
    }
}}
