#pragma once
#include <string>

namespace swizzle { namespace lexer {
    class TokenInfo;
}}

namespace swizzle { namespace parser {
    struct ParserStateContext;
}}

namespace swizzle { namespace types { namespace utils {

    // look at @token to determine if it is an enum value in the form namespace::namespace2::EnumType::Value
    // which exists as a value on @enumNode
    bool validateEnumValue(const parser::ParserStateContext& context, const std::string& enumType, const lexer::TokenInfo& token);
}}}
