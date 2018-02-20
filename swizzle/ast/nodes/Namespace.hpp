#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class Namespace : public Node
    {
    public:
        Namespace(const lexer::TokenInfo& name);
        const lexer::TokenInfo& name() const;

        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth = Node::Depth::All) override;
        
    private:
        const lexer::TokenInfo name_;
    };
}}}
