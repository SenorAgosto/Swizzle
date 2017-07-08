#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

#include <string>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast { namespace nodes {

    class VariableBlock : public Node
    {
    public:
        VariableBlock(const lexer::TokenInfo& variableBlockInfo);

        const lexer::TokenInfo& variableBlockInfo() const;

        void variableOnField(const lexer::TokenInfo& variableOnField);
        const lexer::TokenInfo& variableOnField() const;

        void accept(VisitorInterface& visitor) override;

    private:
        const lexer::TokenInfo variableBlockInfo_;   // variable_block keyword
        lexer::TokenInfo variableOnFieldInfo_;       // field we're variable on
    };
}}}
