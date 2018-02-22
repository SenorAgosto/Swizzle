#include <swizzle/ast/nodes/Extern.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
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

    void Extern::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth)
    {
        visitor(ancestors, *this);
        
        if(depth == Depth::One) return;
        ancestors.push(*this);
        
        for(auto& child : children())
        {
            child->accept(visitor, ancestors, depth);
        }
        
        ancestors.pop();
    }
}}}
