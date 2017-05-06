#include <swizzle/lexer/TokenType.hpp>

namespace swizzle { namespace lexer {

    std::ostream& operator<<(std::ostream& os, const TokenType type)
    {
        switch(type)
        {
        case TokenType::string:                 return os << "TokenType::string";
        case TokenType::string_literal:         return os << "TokenType::string_literal";
        case TokenType::char_literal:           return os << "TokenType::char_literal";
        case TokenType::numeric_literal:        return os << "TokenType::numeric_literal";
        case TokenType::attribute:              return os << "TokenType::attribute";
        case TokenType::keyword:                return os << "TokenType::keyword";

        case TokenType::l_brace:                return os << "TokenType::l_brace";
        case TokenType::r_brace:                return os << "TokenType::r_brace";
        case TokenType::l_bracket:              return os << "TokenType::l_bracket";
        case TokenType::r_bracket:              return os << "TokenType::r_bracket";
        case TokenType::equal:                  return os << "TokenType::equal";
        case TokenType::end_statement:          return os << "TokenType::end_statement";
        case TokenType::underlying_type:        return os << "TokenType::underlying_type";
        case TokenType::scope_operator:         return os << "TokenType::scope_operator";
        case TokenType::comment:                return os << "TokenType::comment";
        case TokenType::multiline_comment:      return os << "TokenType::multiline_comment";

        default:                                return os << "Unknown TokenType Value";
        };
    }
}}
