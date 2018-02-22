#include <swizzle/ast/nodes/Namespace.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Namespace::Namespace(const lexer::TokenInfo& name)
        : name_(name)
    {
    }

    const lexer::TokenInfo& Namespace::name() const
    {
        return name_;
    }

    void Namespace::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Depth depth)
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
