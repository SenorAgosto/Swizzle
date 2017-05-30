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
        Token(const boost::string_view& value, const std::size_t position, const std::size_t length, const TokenType type);

        void type(const TokenType type) { type_ = type; }
        TokenType type() const { return type_; }

        // extend the token to the right by @count bytes
        void expand(const std::size_t count = 1) { length_ += count; }
        void contract(const std::size_t count = 1) { length_ -= count; }

        bool empty() const;

        boost::string_view value() const { return value_.substr(position_, length_); }
        std::string to_string() const { return value_.substr(position_, length_).to_string(); }

    private:
        boost::string_view value_;

        std::size_t position_;
        std::size_t length_;

        TokenType type_;
    };
}}
