#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class FieldLabel : public Node
    {
    public:
        FieldLabel(const lexer::TokenInfo& label);

        const lexer::TokenInfo& label() const;

        void colon(const lexer::TokenInfo& colon);
        const lexer::TokenInfo& colon() const;

        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth = Node::Depth::All) override;

    private:
        const lexer::TokenInfo label_;
        lexer::TokenInfo colon_;
    };
}}}
