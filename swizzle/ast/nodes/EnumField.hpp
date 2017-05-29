#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

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

        bool isNegative() const;
        std::size_t value() const;

    private:
        const lexer::TokenInfo name_;
        const lexer::TokenInfo underlying_;

        lexer::TokenInfo valueInfo_;

        std::size_t value_;
        bool isNegative_;
    };
}}}
