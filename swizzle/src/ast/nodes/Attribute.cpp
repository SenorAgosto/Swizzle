#include <swizzle/ast/nodes/Attribute.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
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

    void Attribute::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth)
    {
        visitor(ancestors, *this);
        
        if(depth == Depth::One) return;
        ancestors.push(*this);
        
        for(auto& child : children())
        {
            auto parent = this;
            child->accept(visitor, ancestors, depth);
        }
        
        ancestors.pop();
    }

}}}
