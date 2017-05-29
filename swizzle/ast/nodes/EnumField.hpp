#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class EnumField : public Node
    {
    public:
        EnumField(const lexer::TokenInfo& info, const lexer::TokenInfo& underlyingType);

        const lexer::TokenInfo& name() const;
        const lexer::TokenInfo& underlyingType() const;

        void value(const lexer::TokenInfo& val);
        const lexer::TokenInfo& value() const;

    private:
        const lexer::TokenInfo name_;
        const lexer::TokenInfo underlyingType_;

        lexer::TokenInfo value_;
    };
}}}
