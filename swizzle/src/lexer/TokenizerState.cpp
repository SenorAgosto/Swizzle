#include <swizzle/lexer/TokenizerState.hpp>

namespace swizzle { namespace lexer {

    std::ostream& operator<<(std::ostream& os, const TokenizerState state)
    {
        switch(state)
        {
        case TokenizerState::Init:                          return os << "TokenizerState::Init";
        case TokenizerState::FirstSlash:                    return os << "TokenizerState::FirstSlash";
        case TokenizerState::Comment:                       return os << "TokenizerState::Comment";
        case TokenizerState::MultilineComment:              return os << "TokenizerState::MultilineComment";
        case TokenizerState::StringLiteral:                 return os << "TokenizerState::StringLiteral";
        case TokenizerState::CharLiteral:                   return os << "TokenizerState::CharLiteral";
        case TokenizerState::NumericLiteral:                return os << "TokenizerState::NumericLiteral";
        case TokenizerState::EscapedCharInCharLiteral:      return os << "TokenizerState::EscapedCharInCharLiteral";
        case TokenizerState::EndCharLiteral:                return os << "TokenizerState::EndCharLiteral";
        case TokenizerState::EscapedCharInStringLiteral:    return os << "TokenizerState::EscapedCharInStringLiteral";
        case TokenizerState::BeginString:                   return os << "TokenizerState::BeginString";
        case TokenizerState::SyntaxError:                   return os << "TokenizerState::SyntaxError";
        default:                                            return os << "Unknown TokenizerState value";
        };
    }
}}
