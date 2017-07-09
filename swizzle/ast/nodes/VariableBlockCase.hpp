#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class VariableBlockCase : public Node
    {
    public:
        void value(const lexer::TokenInfo& name);
        const lexer::TokenInfo& value() const;

        void type(const lexer::TokenInfo& type);
        lexer::TokenInfo type() const;

    private:
        lexer::TokenInfo value_;
        lexer::TokenInfo type_;
    };
}}}
