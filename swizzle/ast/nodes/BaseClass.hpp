#pragma once
#include <swizzle/ast/nodes/Struct.hpp>
#include <swizzle/lexer/TokenInfo.hpp>

namespace swizzle { namespace ast { namespace nodes {

    class BaseClass : public Struct
    {
    public:
        BaseClass(const Struct& str);

    public:
        void accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth = Node::Depth::All) override;
    };
}}}

