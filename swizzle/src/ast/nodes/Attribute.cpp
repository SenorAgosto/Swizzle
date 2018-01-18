#include <swizzle/ast/nodes/Attribute.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Attribute::Attribute(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& Attribute::info() const
    {
        return info_;
    }

    void Attribute::accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth)
    {
        visitor(parent, *this);
        if(depth == Depth::One) return;
        
        for(auto& child : children())
        {
            auto parent = this;
            child->accept(visitor, *parent, depth);
        }
    }

}}}
