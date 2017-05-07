#include <swizzle/IsKeyword.hpp>
#include <regex>

namespace swizzle {

    static const std::regex keywords("import|namespace|using|struct|enum|variable_block|u8|i8|u16|i16|u32|i32|u64|i64|bitfield|const", std::regex::optimize);

    bool IsKeyword(const boost::string_view& token)
    {
        return std::regex_match(token.begin(), token.end(), keywords);
    }
}
