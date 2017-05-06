#pragma once

#include <swizzle/lexer/FileInfo.hpp>
#include <swizzle/lexer/TokenType.hpp>

#include <cstddef>
#include <boost/utility/string_view.hpp>

namespace swizzle { namespace lexer {

    class Token
    {
    public:
        Token();
        Token(const boost::string_view& value, const TokenType type);

        void type(const TokenType type) { type_ = type; }
        TokenType type() const { return type_; }

        void expand(const boost::string_view& source, const std::size_t count = 1);

        bool empty() const;

        const boost::string_view& value() const { return value_; }
        std::string to_string() const { return value_.to_string(); }
        
        void clear();
        
    private:
        boost::string_view value_;
        TokenType type_;
    };
}}
