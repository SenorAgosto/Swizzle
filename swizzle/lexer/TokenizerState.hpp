#pragma once 
#include <cstdint>
#include <ostream>

namespace swizzle { namespace lexer {

    enum class TokenizerState : std::uint8_t {
        Init,
        FirstSlash,
        Comment,
        MultilineComment,
        StringLiteral,
        CharLiteral,
        NumericLiteral,
        EscapedCharInCharLiteral,
        EndCharLiteral,
        EscapedCharInStringLiteral,
        BeginString,
        SyntaxError,
    };

    std::ostream& operator<<(std::ostream& os, const TokenizerState state);
}}
