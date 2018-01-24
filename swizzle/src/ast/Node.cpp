#include <swizzle/ast/Node.hpp>

#include <swizzle/ast/AncestorInfo.hpp>
#include <swizzle/ast/VisitorInterface.hpp>

namespace swizzle { namespace ast {

    const std::deque<Node::smartptr>& Node::children() const
    {
        return children_;
    }

    void Node::append(Node::smartptr node)
    {
        children_.push_back(node);
    }

    bool Node::empty() const
    {
        return children_.empty();
    }

    void Node::accept(VisitorInterface& visitor, AncestorInfo& ancestors, const Node::Depth depth)
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
}}
