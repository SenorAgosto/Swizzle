#include <swizzle/ast/nodes/NumericLiteral.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    NumericLiteral::NumericLiteral(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& NumericLiteral::info() const
    {
        return info_;
    }

    void NumericLiteral::accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth)
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
