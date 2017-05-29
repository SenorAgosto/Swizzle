#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>
#include <swizzle/types/EnumValue.hpp>
#include <swizzle/types/EnumValueType.hpp>

#include <cstddef>

namespace swizzle { namespace ast { namespace nodes {

    class EnumField : public Node
    {
    public:
        EnumField(const lexer::TokenInfo& name, const lexer::TokenInfo& underlyingType);

        const lexer::TokenInfo& name() const;
        const lexer::TokenInfo& underlying() const;

        void valueInfo(const lexer::TokenInfo& val);
        const lexer::TokenInfo& valueInfo() const;

        void value(const types::EnumValueType& value);
        const types::EnumValueType& value() const;

    private:
        const lexer::TokenInfo name_;
        const lexer::TokenInfo underlying_;

        lexer::TokenInfo valueInfo_;

        types::EnumValue value_;
    };
}}}
