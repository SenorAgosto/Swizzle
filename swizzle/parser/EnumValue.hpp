#pragma once 
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/parser/EnumValueType.hpp>

namespace swizzle { namespace parser {

    class EnumValue
    {
    public:
        EnumValue();
        EnumValue(const lexer::TokenInfo& info, const lexer::TokenInfo& underlying, const lexer::TokenInfo& value);
        EnumValue(const lexer::TokenInfo& info, const lexer::TokenInfo& underlying, const EnumValueType& value);

        const EnumValueType& value() const;
        void increment();

    private:
        const lexer::TokenInfo info_;        // actual "value" token
        const lexer::TokenInfo underlying_;  // underyling type token

        EnumValueType value_;
    };
}}
