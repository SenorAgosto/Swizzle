#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast { namespace nodes {

    class HexLiteral : public Node
    {
    public:
        HexLiteral(const lexer::TokenInfo& info);

        const lexer::TokenInfo& info() const;

        void accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth = Node::Depth::All) override;
        
    private:
        const lexer::TokenInfo info_;
    };
}}}
