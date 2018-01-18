#include <swizzle/ast/nodes/Comment.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast { namespace nodes {

    Comment::Comment(const lexer::TokenInfo& info)
        : info_(info)
    {
    }

    const lexer::TokenInfo& Comment::info() const
    {
        return info_;
    }

    void Comment::accept(VisitorInterface& visitor, Node& parent, const Node::Depth depth)
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
