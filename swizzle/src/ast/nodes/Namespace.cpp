#include <swizzle/ast/nodes/Namespace.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Namespace::Namespace(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& Namespace::info() const
    {
        return info_;
    }

    void Namespace::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth)
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
