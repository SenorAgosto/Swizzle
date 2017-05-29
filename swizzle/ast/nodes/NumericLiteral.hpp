#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class NumericLiteral : public Node
    {
    public:
        NumericLiteral(const lexer::TokenInfo& info);

        const lexer::TokenInfo& info() const;

    private:
        const lexer::TokenInfo info_;
    };
}}}
