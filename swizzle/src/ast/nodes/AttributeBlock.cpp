#include <swizzle/ast/nodes/AttributeBlock.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    AttributeBlock::AttributeBlock(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& AttributeBlock::info() const
    {
        return info_;
    }

    void AttributeBlock::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth)
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
