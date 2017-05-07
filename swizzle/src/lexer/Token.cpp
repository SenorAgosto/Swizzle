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
        if(value_.empty())
        {
            value_ = source.substr(0, count);
            return;
        }

        // NOTE: substr() truncates if count extends past the end of the buffer
        value_ = source.substr(std::distance(source.cbegin(), value_.cbegin()), value_.length() + count);
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
