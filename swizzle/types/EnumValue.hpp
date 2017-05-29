#pragma once 
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/EnumValueType.hpp>

namespace swizzle { namespace types {

    class EnumValue
    {
    public:
        EnumValue();
        EnumValue(const lexer::TokenInfo& underlying, const lexer::TokenInfo& value);
        EnumValue(const EnumValueType& value);

        const EnumValueType& value() const;
        void increment();   // throws if value gets out of range

    private:
        EnumValueType value_;
    };
}}
