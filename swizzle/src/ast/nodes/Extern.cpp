#include <swizzle/ast/nodes/Extern.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Extern::Extern(const lexer::TokenInfo& externType)
        : externType_(externType)
    {
    }

    const lexer::TokenInfo& Extern::externType() const
    {
        return externType_;
    }

    void Extern::accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth)
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
