#include <swizzle/lexer/TokenType.hpp>
#include <swizzle/Exceptions.hpp>

namespace swizzle { namespace lexer {

    std::ostream& operator<<(std::ostream& os, const TokenType type)
    {
        switch(type)
        {
        case TokenType::string:                 return os << "TokenType::string";
        case TokenType::string_literal:         return os << "TokenType::string_literal";
        case TokenType::char_literal:           return os << "TokenType::char_literal";
        case TokenType::numeric_literal:        return os << "TokenType::numeric_literal";
        case TokenType::hex_literal:            return os << "TokenType::hex_literal";
        
        case TokenType::attribute:              return os << "TokenType::attribute";
        case TokenType::attribute_value:        return os << "TokenType::attribute_value";
        case TokenType::attribute_block:        return os << "TokenType::attribute_block";

        case TokenType::keyword:                return os << "TokenType::keyword";
        case TokenType::type:                   return os << "TokenType::type";

        case TokenType::l_brace:                return os << "TokenType::l_brace";
        case TokenType::r_brace:                return os << "TokenType::r_brace";
        case TokenType::l_bracket:              return os << "TokenType::l_bracket";
        case TokenType::r_bracket:              return os << "TokenType::r_bracket";
        case TokenType::equal:                  return os << "TokenType::equal";
        case TokenType::end_statement:          return os << "TokenType::end_statement";
        case TokenType::colon:                  return os << "TokenType::colon";
        case TokenType::dot:                    return os << "TokenType::dot";
        case TokenType::comma:                  return os << "TokenType::comma";
        case TokenType::comment:                return os << "TokenType::comment";
        case TokenType::multiline_comment:      return os << "TokenType::multiline_comment";
        case TokenType::whitespace:             return os << "TokenType::whitespace";

        default:                                return os << "Unknown TokenType Value";
        };
    }

    TokenType CharToTokenType(const char c)
    {
        switch(c)
        {
        case '{':   return TokenType::l_brace;
        case '}':   return TokenType::r_brace;
        case '[':   return TokenType::l_bracket;
        case ']':   return TokenType::r_bracket;
        case '=':   return TokenType::equal;
        case ';':   return TokenType::end_statement;
        case '@':   return TokenType::attribute;
        case '.':   return TokenType::dot;
        case ':':   return TokenType::colon;
        case ',':   return TokenType::comma;

        default: break;
        };

        throw TokenizerError("Unknown TokenType character: " + std::string(1, c));
    }
}}
