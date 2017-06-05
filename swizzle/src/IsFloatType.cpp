#include <swizzle/IsFloatType.hpp>
#include <regex>

namespace swizzle {

    namespace {
        static const std::regex keywords("f32|f64", std::regex::optimize);
    }
    
    bool IsFloatType(const boost::string_view& token)
    {
        return std::regex_match(token.begin(), token.end(), keywords);
    }
}
