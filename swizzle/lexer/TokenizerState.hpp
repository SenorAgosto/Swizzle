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
        FloatingPointLiteral,
        NumericLiteral,
        BeginHexLiteral,
        HexLiteral,
        EscapedCharInCharLiteral,
        EndCharLiteral,
        EscapedCharInStringLiteral,
        BeginString,
        Attribute,
        AttributeBlock,
    };

    std::ostream& operator<<(std::ostream& os, const TokenizerState state);
}}
