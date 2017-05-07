#pragma once
#include <swizzle/lexer/Token.hpp>
#include <swizzle/lexer/TokenType.hpp>

#include <boost/utility/string_view.hpp>
#include <cstddef>

namespace swizzle { namespace lexer {

    Token ResetToken(const boost::string_view& source, const std::size_t position);
    Token ResetToken(const boost::string_view& source, const std::size_t position, const TokenType tokenType);
}}
