#include <swizzle/lexer/ResetToken.hpp>
#include <swizzle/lexer/TokenType.hpp>

namespace swizzle { namespace lexer {

    Token ResetToken(const boost::string_view& source, const std::size_t position)
    {
        return Token(source.substr(position, 1), TokenType::string);
    }
}}
