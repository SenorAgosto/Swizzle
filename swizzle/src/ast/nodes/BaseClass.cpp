#include <swizzle/ast/nodes/BaseClass.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    BaseClass::BaseClass(const lexer::TokenInfo& name)
        : name_(name)
    {
    }

    const lexer::TokenInfo& BaseClass::name() const
    {
        return name_;
    }

    void BaseClass::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth)
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
