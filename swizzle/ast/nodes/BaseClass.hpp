#pragma once
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class BaseClass : public Node
    {
    public:
        BaseClass(const lexer::TokenInfo& name);

        // return base class token
        const lexer::TokenInfo& name() const;

        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth = Node::Depth::All) override;

    private:
        const lexer::TokenInfo name_;
    };
}}}

