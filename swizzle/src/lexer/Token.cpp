#include <swizzle/lexer/Token.hpp>
#include <stdexcept>

namespace swizzle { namespace lexer {
    
    Token::Token()
        : type_(TokenType::string)
    {
    }
    
    Token::Token(const boost::string_view& value, const TokenType type)
        : value_(value)
        , type_(type)
    {
    }

    void Token::expand(const boost::string_view& source, const std::size_t count)
    {
        const std::size_t newLength = value_.length() + count;
        if(newLength > source.length())
        {
            throw std::out_of_range("newLength is larger than source std::string_view.");
        }

        value_ = boost::string_view(value_.begin(), newLength);
    }

    bool Token::empty() const
    {
        if(type_ == TokenType::string_literal)
        {
            return false;
        }

        const auto spot = value_.find_first_not_of("\n\r\t ");
        return spot == boost::string_view::npos;
    }
    
    void Token::clear()
    {
        value_.clear();
        type_ = TokenType::string;
    }
}}
