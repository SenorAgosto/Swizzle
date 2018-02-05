#include <swizzle/types/utils/SetValueFromChar.hpp>

#include <swizzle/Exceptions.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace types { namespace utils {

    std::uint64_t setValueFromChar(const boost::string_view& underlying, const lexer::TokenInfo& token)
    {
        if(token.token().value().length() != 3)
        {
            throw ParserError("Character literal value has unexpected length");
        }
        
        if((underlying == "u8") || (underlying == "i8")   ||
           (underlying == "u16") || (underlying == "i16") ||
           (underlying == "u32") || (underlying == "i32") ||
           (underlying == "u64") || (underlying == "i64"))
        {
            return token.token().value().at(1);
        }

        throw SyntaxError("Encountered unknown enum type:" + underlying.to_string(), token);
    }
}}}
