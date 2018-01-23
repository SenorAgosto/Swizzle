#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast {
    class VisitorInterface;
}}

namespace swizzle { namespace ast { namespace nodes {

    class Extern : public Node
    {
    public:
        Extern(const lexer::TokenInfo& externType);
        const lexer::TokenInfo& externType() const;

        void accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth = Node::Depth::All) override;
        
    private:
        const lexer::TokenInfo externType_;
    };
}}}
