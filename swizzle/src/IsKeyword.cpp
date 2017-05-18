#include <swizzle/IsKeyword.hpp>
#include <regex>

namespace swizzle {

    static const std::regex keywords("const|case|enum|import|namespace|struct|using", std::regex::optimize);

    bool IsKeyword(const boost::string_view& token)
    {
        return std::regex_match(token.begin(), token.end(), keywords);
    }
}
