#pragma once 
#include <swizzle/ast/Node.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class Root : public Node
    {
    public:
        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth = Node::Depth::All) override;
    };
}}}
