#include <swizzle/ast/nodes/HexLiteral.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    HexLiteral::HexLiteral(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& HexLiteral::info() const
    {
        return info_;
    }

    void HexLiteral::accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth)
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
