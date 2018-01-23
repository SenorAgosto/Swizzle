#include <swizzle/ast/nodes/MultilineComment.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    MultilineComment::MultilineComment(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& MultilineComment::info() const
    {
        return info_;
    }

    void MultilineComment::accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth)
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
