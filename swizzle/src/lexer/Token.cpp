#include <swizzle/lexer/Token.hpp>
#include <stdexcept>

namespace swizzle { namespace lexer {
    
    Token::Token()
        : position_(0)
        , length_(0)
        , type_(TokenType::string)
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
    
    boost::string_view Token::line() const
    {
        if(value_.empty()) return value_;

        std::size_t start = position_;
        while((start > 0) && value_.at(start) != '\n') --start;
        
        std::size_t end = position_;
        const std::size_t fin = value_.length();
        while((end < fin) && value_.at(end) != '\n') ++end;
        
        return value_.substr(start, end - start);
    }
}}
