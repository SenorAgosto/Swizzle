#include <swizzle/lexer/Token.hpp>
#include <stdexcept>

namespace swizzle { namespace lexer {
    
    Token::Token()
        : type_(TokenType::string)
    {
    }
    
    Token::Token(const boost::string_view& value, const std::size_t position, const std::size_t length, const TokenType type)
        : value_(value)
        , position_(position)
        , length_(length)
        , type_(type)
    {
    }

    bool Token::empty() const
    {
        if(type_ == TokenType::string_literal)
        {
            return false;
        }

        if(type_ == TokenType::whitespace)
        {
            return true;
        }

        const auto spot = value_.substr(position_, length_).find_first_not_of("\n\r\t ");
        return spot == boost::string_view::npos;
    }
}}
